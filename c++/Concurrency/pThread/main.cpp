void* print_number(void* arg) {
    int num = *(int*)arg;
    std::cout << "Number: " << num << "\n";
    return nullptr;
}

int main() {
    pthread_t threads[5];
    float neel = 2.2;
    std::string ka = "ka";
    int numbers[] = {1, 2, 3, 4, 5};
    
    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], nullptr, print_number, &numbers[i] , ka , neel);
    }
    
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], nullptr);
    }
}