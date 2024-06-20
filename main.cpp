#include <iostream>
using namespace std;

template <typename T>
class SharedPtr {
   private:
    T* _ptr;
    size_t* _ref_count;

    void increaseRefCount() {
        if (_ref_count) {
            ++(*_ref_count);
        }
    }

    void decreaseRefCount() {
        if (_ref_count && --(*_ref_count) == 0) {
            delete _ptr;
            delete _ref_count;
        }
    }

   public:
    explicit SharedPtr(T* ptr = nullptr): _ptr(ptr), _ref_count(new size_t(1)) {}

    ~SharedPtr() {
        decreaseRefCount();
    }

    SharedPtr(const SharedPtr<T>& other): _ptr(other._ptr), _ref_count(other._ref_count) {
        increaseRefCount();
    }

    size_t count() {
        return *_ref_count;
    }

    SharedPtr<T>& operator=(const SharedPtr<T>& other) {
        if (this != &other) {
            decreaseRefCount();
            _ptr = other._ptr;
            _ref_count = other._ref_count;
            increaseRefCount();
        }
        return *this;
    }

    T& operator*() const {
        return *_ptr;
    }

    T* operator->() const {
        return _ptr;
    }
};

int main() {
    int* ptr = new int(1);
    SharedPtr<int> ptr1(ptr);
    cout << ptr1.count() << " " << *ptr1 << "\n";

    SharedPtr<int> ptr2 = ptr1;
    cout << ptr2.count() << " " << *ptr2 << "\n";

    {
        SharedPtr<int> ptr3 = ptr1;
        cout << ptr3.count() << " " << *ptr3 << "\n";
    }
    
    cout << ptr1.count() << " " << *ptr1 << " " << *ptr2 <<  "\n";

    return 0;
}