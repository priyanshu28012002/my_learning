#include <iostream>
#include <utility> // for std::move, std::swap

// Interface for all smart pointers (Dependency Inversion Principle)
template <typename T>
class SPTR
{
public:
    virtual ~SPTR() = default;
    virtual T &operator*() const = 0;
    virtual T *operator->() const = 0;
    virtual explicit operator bool() const = 0;
    virtual T *get() const = 0;
};

// Single Responsibility Principle - manages exclusive ownership
template <typename T>
class UniquePtr : public SPTR<T>
{
    T *ptr;

    void cleanup()
    {
        if (ptr)
        {
            delete ptr;
            ptr = nullptr;
        }
    }

public:
    // Constructor
    explicit UniquePtr(T *p = nullptr) : ptr(p) {}

    // Prevent copying (Open/Closed Principle - open for extension, closed for modification)
    UniquePtr(const UniquePtr &) = delete;
    UniquePtr &operator=(const UniquePtr &) = delete;

    // Move semantics
    UniquePtr(UniquePtr &&other) noexcept : ptr(other.ptr)
    {
        other.ptr = nullptr;
    }

    UniquePtr &operator=(UniquePtr &&other) noexcept
    {
        if (this != &other)
        {
            cleanup();
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    // Destructor
    ~UniquePtr()
    {
        cleanup();
    }

    // Interface implementation
    T &operator*() const override { return *ptr; }
    T *operator->() const override { return ptr; }
    explicit operator bool() const override { return ptr != nullptr; }
    T *get() const override { return ptr; }

    // Release ownership
    T *release()
    {
        T *temp = ptr;
        ptr = nullptr;
        return temp;
    }

    // Reset with new pointer
    void reset(T *p = nullptr)
    {
        cleanup();
        ptr = p;
    }

    // Swap with another UniquePtr
    void swap(UniquePtr &other)
    {
        std::swap(ptr, other.ptr);
    }
};

// Reference counting control block
template <typename T>
class ControlBlock
{
    T *ptr;
    size_t shared_count;
    size_t weak_count;

public:
    explicit ControlBlock(T *p) : ptr(p), shared_count(1), weak_count(0) {}

    void increment_shared() { ++shared_count; }
    void decrement_shared() { --shared_count; }
    void increment_weak() { ++weak_count; }
    void decrement_weak() { --weak_count; }

    size_t use_count() const { return shared_count; }
    size_t weak_count() const { return weak_count; }

    T *get() { return ptr; }

    void destroy()
    {
        if (ptr)
        {
            delete ptr;
            ptr = nullptr;
        }
    }

    ~ControlBlock()
    {
        destroy();
    }
};

// SharedPtr implementation
template <typename T>
class SharedPtr : public SPTR<T>
{
    ControlBlock<T> *control_block;

    void cleanup()
    {
        if (control_block)
        {
            control_block->decrement_shared();
            if (control_block->use_count() == 0)
            {
                control_block->destroy();
                if (control_block->weak_count() == 0)
                {
                    delete control_block;
                }
            }
            control_block = nullptr;
        }
    }

public:
    // Constructors
    explicit SharedPtr(T *ptr = nullptr) : control_block(ptr ? new ControlBlock<T>(ptr) : nullptr) {}

    // Copy semantics
    SharedPtr(const SharedPtr &other) : control_block(other.control_block)
    {
        if (control_block)
        {
            control_block->increment_shared();
        }
    }

    SharedPtr &operator=(const SharedPtr &other)
    {
        if (this != &other)
        {
            cleanup();
            control_block = other.control_block;
            if (control_block)
            {
                control_block->increment_shared();
            }
        }
        return *this;
    }

    // Move semantics
    SharedPtr(SharedPtr &&other) noexcept : control_block(other.control_block)
    {
        other.control_block = nullptr;
    }

    SharedPtr &operator=(SharedPtr &&other) noexcept
    {
        if (this != &other)
        {
            cleanup();
            control_block = other.control_block;
            other.control_block = nullptr;
        }
        return *this;
    }

    // Destructor
    ~SharedPtr()
    {
        cleanup();
    }

    // Interface implementation
    T &operator*() const override { return *control_block->get(); }
    T *operator->() const override { return control_block->get(); }
    explicit operator bool() const override { return control_block && control_block->get(); }
    T *get() const override { return control_block ? control_block->get() : nullptr; }

    // SharedPtr specific functions
    size_t use_count() const
    {
        return control_block ? control_block->use_count() : 0;
    }

    void reset(T *ptr = nullptr)
    {
        cleanup();
        control_block = ptr ? new ControlBlock<T>(ptr) : nullptr;
    }
};

template <typename T>
class WeakPtr
{
    ControlBlock<T> *control_block;

    void cleanup()
    {
        if (control_block)
        {
            control_block->decrement_weak();
            if (control_block->use_count() == 0 && control_block->weak_count() == 0)
            {
                delete control_block;
            }
            control_block = nullptr;
        }
    }

public:
    // Constructors
    WeakPtr() : control_block(nullptr) {}

    WeakPtr(const SharedPtr<T> &shared) : control_block(shared.control_block)
    {
        if (control_block)
        {
            control_block->increment_weak();
        }
    }

    // Copy semantics
    WeakPtr(const WeakPtr &other) : control_block(other.control_block)
    {
        if (control_block)
        {
            control_block->increment_weak();
        }
    }

    WeakPtr &operator=(const WeakPtr &other)
    {
        if (this != &other)
        {
            cleanup();
            control_block = other.control_block;
            if (control_block)
            {
                control_block->increment_weak();
            }
        }
        return *this;
    }

    // Move semantics
    WeakPtr(WeakPtr &&other) noexcept : control_block(other.control_block)
    {
        other.control_block = nullptr;
    }

    WeakPtr &operator=(WeakPtr &&other) noexcept
    {
        if (this != &other)
        {
            cleanup();
            control_block = other.control_block;
            other.control_block = nullptr;
        }
        return *this;
    }

    // Destructor
    ~WeakPtr()
    {
        cleanup();
    }

    // Check if expired
    bool expired() const
    {
        return !control_block || control_block->use_count() == 0;
    }

    // Try to create a SharedPtr
    SharedPtr<T> lock() const
    {
        if (expired())
        {
            return SharedPtr<T>();
        }
        return SharedPtr<T>(*this);
    }

    size_t use_count() const
    {
        return control_block ? control_block->use_count() : 0;
    }
};

int main()
{
    // UniquePtr usage
    UniquePtr<int> uptr(new int(10));
    std::cout << "UniquePtr value: " << *uptr << std::endl;

    // SharedPtr usage
    SharedPtr<int> sptr1(new int(20));
    {
        SharedPtr<int> sptr2 = sptr1;
        std::cout << "SharedPtr use count: " << sptr1.use_count() << std::endl;
    }
    std::cout << "SharedPtr use count after scope: " << sptr1.use_count() << std::endl;

    // WeakPtr usage
    WeakPtr<int> wptr(sptr1);
    if (!wptr.expired())
    {
        SharedPtr<int> sptr3 = wptr.lock();
        std::cout << "WeakPtr locked value: " << *sptr3 << std::endl;
    }

    return 0;
}