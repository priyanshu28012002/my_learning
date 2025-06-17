# **Comprehensive Guide to C++ STL Containers and Algorithms**

Here's a detailed overview of key C++ Standard Template Library (STL) components, including containers, algorithms, and utility functions:

## **1. Sequence Containers**
### **(1) `std::vector`**
- **Dynamic array** with contiguous storage
- **Fast random access** (O(1))
- **Insertion/Deletion at end**: O(1) amortized
- **Insertion/Deletion in middle**: O(n)
- **Example**:
  ```cpp
  std::vector<int> v = {1, 2, 3};
  v.push_back(4); // 1,2,3,4
  ```

### **(2) `std::deque` (Double-Ended Queue)**
- **Dynamic array** that grows at both ends
- **Fast insertion/removal at both ends** (O(1))
- **Slower than vector** for middle operations
- **Example**:
  ```cpp
  std::deque<int> d = {1, 2};
  d.push_front(0); // 0,1,2
  ```

### **(3) `std::list` (Doubly-Linked List)**
- **Non-contiguous memory**
- **Fast insertion/deletion anywhere** (O(1))
- **No random access** (O(n) traversal)
- **Example**:
  ```cpp
  std::list<int> l = {1, 2};
  l.push_back(3); // 1,2,3
  ```

### **(4) `std::forward_list` (Singly-Linked List)**
- **Lighter than `list`** (one link per node)
- **Only forward traversal**
- **Example**:
  ```cpp
  std::forward_list<int> fl = {1, 2};
  fl.push_front(0); // 0,1,2
  ```

---

## **2. Associative Containers**
### **(1) `std::set`**
- **Unique keys**, sorted (Red-Black Tree)
- **Insert/Find/Delete**: O(log n)
- **Example**:
  ```cpp
  std::set<int> s = {3, 1, 2}; // 1,2,3
  s.insert(4); // 1,2,3,4
  ```

### **(2) `std::multiset`**
- **Allows duplicate keys**, sorted
- **Example**:
  ```cpp
  std::multiset<int> ms = {1, 1, 2}; // 1,1,2
  ```

### **(3) `std::unordered_set` (Hash Set)**
- **Unique keys**, unsorted (Hash Table)
- **Average O(1) operations**, worst-case O(n)
- **Example**:
  ```cpp
  std::unordered_set<int> us = {3, 1, 2}; // Unordered
  ```

### **(4) `std::unordered_multiset`**
- **Allows duplicates**, unsorted (Hash Table)
- **Example**:
  ```cpp
  std::unordered_multiset<int> ums = {1, 1, 2};
  ```

---

## **3. Associative Key-Value Containers**
### **(1) `std::map` (Sorted Key-Value Pairs)**
- **Unique keys**, sorted (Red-Black Tree)
- **Example**:
  ```cpp
  std::map<std::string, int> m = {{"Alice", 25}, {"Bob", 30}};
  m["Charlie"] = 35;
  ```

### **(2) `std::multimap`**
- **Allows duplicate keys**, sorted
- **Example**:
  ```cpp
  std::multimap<std::string, int> mm = {{"A", 1}, {"A", 2}};
  ```

### **(3) `std::unordered_map` (Hash Map)**
- **Unique keys**, unsorted (Hash Table)
- **Example**:
  ```cpp
  std::unordered_map<std::string, int> um = {{"A", 1}, {"B", 2}};
  ```

### **(4) `std::unordered_multimap`**
- **Allows duplicates**, unsorted (Hash Table)
- **Example**:
  ```cpp
  std::unordered_multimap<std::string, int> umm = {{"A", 1}, {"A", 2}};
  ```

---

## **4. Container Adapters**
### **(1) `std::stack` (LIFO)**
- **Uses `deque` by default**
- **Example**:
  ```cpp
  std::stack<int> s;
  s.push(1); s.push(2); // 2,1
  ```

### **(2) `std::queue` (FIFO)**
- **Uses `deque` by default**
- **Example**:
  ```cpp
  std::queue<int> q;
  q.push(1); q.push(2); // 1,2
  ```

### **(3) `std::priority_queue` (Heap)**
- **Max-heap by default**
- **Example**:
  ```cpp
  std::priority_queue<int> pq;
  pq.push(3); pq.push(1); // 3,1
  ```

---

## **5. Important STL Algorithms**
### **(1) `std::sort()`**
- **Sorts a range** (O(n log n))
- **Example**:
  ```cpp
  std::vector<int> v = {3, 1, 2};
  std::sort(v.begin(), v.end()); // 1,2,3
  ```

### **(2) `std::next_permutation()`**
- **Generates next lexicographical permutation**
- **Example**:
  ```cpp
  std::string s = "abc";
  do { std::cout << s << "\n"; } 
  while (std::next_permutation(s.begin(), s.end()));
  ```

### **(3) `min_element()` / `max_element()`**
- **Finds min/max in a range** (O(n))
- **Example**:
  ```cpp
  auto it = std::max_element(v.begin(), v.end());
  ```

### **(4) `__builtin_popcount()` (GCC)**
- **Counts set bits in an integer**
- **Example**:
  ```cpp
  int x = 5; // 101 (binary)
  int bits = __builtin_popcount(x); // 2
  ```

---

## **6. Other Useful STL Functions**
| Function | Description |
|----------|-------------|
| `std::binary_search()` | Checks if an element exists (O(log n)) |
| `std::lower_bound()` | Finds first element ≥ value |
| `std::upper_bound()` | Finds first element > value |
| `std::rotate()` | Rotates elements in a range |
| `std::reverse()` | Reverses a range |
| `std::accumulate()` | Computes sum of elements |
| `std::find_if()` | Finds element matching a condition |

---

## **7. Key Takeaways**
- **Sequence Containers**: `vector`, `deque`, `list`, `forward_list`
- **Associative Containers**: `set`, `map`, `multiset`, `multimap`
- **Unordered Containers**: `unordered_set`, `unordered_map`, etc.
- **Adapters**: `stack`, `queue`, `priority_queue`
- **Algorithms**: `sort()`, `next_permutation()`, `min_element()`, etc.

# **Advanced C++ STL Containers and Algorithms**

Beyond the standard containers, C++ offers several specialized and powerful STL components. Here's an expanded list with detailed explanations:

## **1. Specialized Containers**

### **(1) `std::array` (Fixed-Size Array)**
- **Fixed-size** (compile-time) alternative to `vector`
- **No dynamic resizing**, stack-allocated
- **Example**:
  ```cpp
  std::array<int, 3> arr = {1, 2, 3};
  ```

### **(2) `std::bitset` (Bit Array)**
- **Fixed-size sequence of bits**
- **Optimized for bitwise operations**
- **Example**:
  ```cpp
  std::bitset<8> bs("10101010");
  bs.flip(2); // Flip bit at position 2
  ```

### **(3) `std::span` (C++20)**
- **Non-owning view** over contiguous memory
- **Lightweight alternative** to passing `vector.data()` + size
- **Example**:
  ```cpp
  std::vector<int> v = {1, 2, 3};
  std::span<int> s(v);
  ```

### **(4) `std::flat_(set|map)` (C++23)**
- **Sorted containers** with contiguous storage
- **Faster lookup** than `set/map` (better cache locality)
- **Example**:
  ```cpp
  std::flat_set<int> fs = {3, 1, 2}; // Stored as 1,2,3
  ```

---

## **2. Advanced Associative Containers**

### **(1) `std::pmr::(unordered_)set/map` (C++17)**
- **Polymorphic allocator** versions
- **Custom memory management**
- **Example**:
  ```cpp
  std::pmr::unordered_map<int, std::pmr::string> pmr_map;
  ```

### **(2) `boost::container::flat_(set|map)`**
- **Boost's version** of C++23's flat containers
- **Available now** in Boost

---

## **3. Advanced Algorithms**

### **(1) Parallel Algorithms (C++17)**
- **Multi-threaded versions** of standard algorithms
- **Example**:
  ```cpp
  std::vector<int> v(1'000'000);
  std::sort(std::execution::par, v.begin(), v.end());
  ```

### **(2) `std::reduce()` (C++17)**
- **Parallelizable version** of `accumulate()`
- **Example**:
  ```cpp
  std::vector<int> v = {1, 2, 3};
  int sum = std::reduce(v.begin(), v.end(), 0);
  ```

### **(3) `std::inclusive_scan()` (C++17)**
- **Parallel prefix sum**
- **Example**:
  ```cpp
  std::vector<int> v = {1, 2, 3};
  std::inclusive_scan(v.begin(), v.end(), v.begin());
  // v becomes [1, 3, 6]
  ```

### **(4) `std::gcd()/std::lcm()` (C++17)**
- **Mathematical utilities**
- **Example**:
  ```cpp
  int x = std::gcd(8, 12); // 4
  int y = std::lcm(4, 6);  // 12
  ```

### **(5) `std::clamp()` (C++17)**
- **Constrains value** between bounds
- **Example**:
  ```cpp
  int x = std::clamp(15, 0, 10); // returns 10
  ```

### **(6) `std::sample()` (C++17)**
- **Random sampling** from a range
- **Example**:
  ```cpp
  std::vector<int> in = {1, 2, 3, 4, 5};
  std::vector<int> out(3);
  std::sample(in.begin(), in.end(), out.begin(), 3, std::mt19937{});
  ```

### **(7) `std::shift_left/shift_right()` (C++20)**
- **Bit shift-like operations** on ranges
- **Example**:
  ```cpp
  std::vector<int> v = {1, 2, 3, 4};
  std::shift_left(v.begin(), v.end(), 1);
  // v becomes [2, 3, 4, ?]
  ```

---

## **4. Range-Based Utilities (C++20)**

### **(1) Ranges Algorithms**
- **Range versions** of standard algorithms
- **Example**:
  ```cpp
  std::vector<int> v = {3, 1, 2};
  std::ranges::sort(v); // No more .begin()/.end()
  ```

### **(2) `std::views` (C++20)**
- **Lazy, composable range adaptors**
- **Example**:
  ```cpp
  std::vector<int> v = {1, 2, 3, 4, 5};
  auto even = v | std::views::filter([](int x) { return x % 2 == 0; });
  // even is a view of [2, 4]
  ```

### **(3) `std::join_view` (C++20)**
- **Flattens** nested ranges
- **Example**:
  ```cpp
  std::vector<std::vector<int>> v = {{1, 2}, {3, 4}};
  for (int x : v | std::views::join) {
      // x will be 1, 2, 3, 4
  }
  ```

---

## **5. Specialized Algorithms**

### **(1) `std::boyer_moore_searcher` (C++17)**
- **Efficient string searching** algorithm
- **Example**:
  ```cpp
  std::string text = "ABABDABACDABABCABAB";
  std::string pat = "ABABCABAB";
  auto searcher = std::boyer_moore_searcher(pat.begin(), pat.end());
  auto it = std::search(text.begin(), text.end(), searcher);
  ```

### **(2) `std::visit` (C++17)**
- **Pattern matching** for `std::variant`
- **Example**:
  ```cpp
  std::variant<int, std::string> v = "hello";
  std::visit([](auto&& arg) {
      std::cout << arg;
  }, v);
  ```

---

## **6. Key Takeaways**
- **C++17/20 introduced** many powerful additions
- **Parallel algorithms** enable multi-threaded processing
- **Ranges/Views** provide cleaner range operations
- **Specialized containers** (`array`, `bitset`) fill niche roles
- **Advanced algorithms** (`sample`, `clamp`) solve common problems

