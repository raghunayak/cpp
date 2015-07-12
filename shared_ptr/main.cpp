/*
 * Simple implementation of shared_ptr.
 * Below code is non thread safe.
 *
 * (c) 2015 Raghavendra Nayak, All Rights Reserved.
 * http://www.openguru.com/
 */

#include <iostream>

namespace my {
    template <class T>
    class shared_ptr {
        T* ptr;
        int* ref_count;

        /**
         * Initializes the ref count used for tracking the usage.
         */
        void initialize_ref_count() {
            if (ref_count != nullptr)
                return;
            try {
                ref_count = new int;
                *ref_count = 1;
            }
            catch (std::bad_alloc e) {
                std::cerr << "Memory allocation error: " << e.what();
            }
        }

    public:
        // Constructors
        shared_ptr()
            : ptr(nullptr), ref_count(nullptr) {}

        shared_ptr(nullptr_t ptr)
            : ptr(ptr), ref_count(nullptr) {
        }

        shared_ptr(T* ptr)
            : ptr(ptr), ref_count(nullptr) {
            initialize_ref_count();
        }

        // Destructor
        ~shared_ptr() {
            --(*ref_count);
            if (*ref_count == 0) {
                delete ref_count;
                ref_count = nullptr;
                delete ptr;
                ptr = nullptr;
            }
        }

        // Copy constructor
        shared_ptr(const shared_ptr& copy) {
            ptr = copy.ptr;
            ref_count = copy.ref_count;
            if (ref_count != nullptr) {
                ++(*ref_count);
            }
        }

        // Assignment operator
        shared_ptr& operator=(const shared_ptr& copy) {
            ptr = copy.ptr;
            ref_count = copy.ref_count;
            if (ref_count != nullptr) {
                ++(*ref_count);
            }
            return *this;
        }

        // Returns stored object reference
        T& operator*() const {
            return *ptr;
        }

        // Returns stored object pointer
        T* operator->() const {
            return ptr;
        }
    };
}

class Base
{
    int data;
public:
    Base() : data(0) { std::cout << "  Base::Base()\n"; }
    virtual ~Base() { std::cout << "  Base::~Base()\n"; }

    void set_data(int data) {
        this->data = data;
    }
    int get_data() const {
        return this->data;
    }
};

class Derived : public Base
{
public:
    Derived() { std::cout << "  Derived::Derived()\n"; }
    ~Derived() { std::cout << "  Derived::~Derived()\n"; }
};

my::shared_ptr<Base> function() 
{
    my::shared_ptr<Base> ptr = new Derived();
    return ptr;
}

int main()
{
    my::shared_ptr<Base> ptr1;
    
    {
        my::shared_ptr<Base> ptr2 = function();
        ptr2->set_data(100);
        my::shared_ptr<Base> ptr3 = ptr2;
        ptr1 = ptr3;
    }
    std::cout << "  data set to: " << (*ptr1).get_data() << std::endl;

    return 0;
}