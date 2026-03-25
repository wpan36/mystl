#pragma once

#include <cstring>
#include <cstddef>

class MyString {
private:
    char* data_;
    std::size_t size_;
    std::size_t capacity_;

public:
    MyString()
        : data_(new char[1]), size_(0), capacity_(0) {
            data_[0] = '\0';
        }
    
    MyString(const char* str) {
        if (str == nullptr) {
            data_ = new char[1];
            data_[0] = '\0';
            size_ = 0;
            capacity_ = 0;
            return;
        }
        
        size_ = std::strlen(str);
        capacity_ = size_;
        data_ = new char[capacity_ + 1];
        std::memcpy(data_, str, size_ + 1);
    }

    MyString(const MyString& other)
        : data_(new char[other.capacity_ + 1]), 
        size_(other.size_), 
        capacity_(other.capacity_) {
            std::memcpy(data_, other.data_, other.size_ + 1);
        }

    MyString& operator=(const MyString& other) {
        if (this == &other) {
            return *this;
        }
        char* new_data = new char[other.capacity_ + 1];
        std::memcpy(new_data, other.data_, other.size_ + 1);
        
        delete[] data_;
        data_ = new_data;
        capacity_ = other.capacity_;
        size_ = other.size_;
        return *this;
    }

    MyString(MyString&& other) noexcept 
        : data_(other.data_),
        size_(other.size_),
        capacity_(other.capacity_) {
            other.data_ = new char[1];
            other.data_[0] = '\0';
            other.size_ = 0;
            other.capacity_ = 0;
    }

    MyString& operator=(MyString&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        delete[] data_;

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        
        other.data_ = new char[1];
        other.data_[0] = '\0';
        other.size_ = 0;
        other.capacity_ = 0;

        return *this;
    }

    ~MyString() {
        delete[] data_;
    }

    std::size_t size() const {
        return size_;
    }

    std::size_t capacity() const {
        return capacity_;
    }

    const char* c_str() const {
        return data_;
    }

    void reserve(std::size_t new_capacity) {
        if (capacity_ >= new_capacity) return;
        char* new_data = new char[new_capacity + 1];
        std::memcpy(new_data, data_, size_ + 1);
        delete[] data_;
        
        data_ = new_data;
        capacity_ = new_capacity;
    }

    void push_back(char ch) {
        if (size_ == capacity_) {
            std::size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            reserve(new_capacity);
        }

        data_[size_] = ch;
        ++size_;
        data_[size_] = '\0';
    }

};