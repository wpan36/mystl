#pragma once

#include <cstring>
#include <cstddef>
#include <stdexcept>

class MyString {
private:
    char* data_;
    std::size_t size_;
    std::size_t capacity_;
    bool is_small_;

    void ensure_capacity(std::size_t required_capacity) {
        if (capacity_ >= required_capacity) return;
        std::size_t new_capacity = capacity_ == 0 ? 1 : capacity_;
        while (new_capacity < required_capacity) {
            new_capacity *= 2;
        }
        reserve(new_capacity);
    }

public:
    MyString() noexcept
        : data_(nullptr), size_(0), capacity_(0) {}
    
    MyString(const char* str) {
        if (str == nullptr || str[0] == '\0') {
            data_ = nullptr;
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
        
        MyString temp(other);
        swap(temp);
        return *this;
    }

    MyString(MyString&& other) noexcept 
        : data_(other.data_),
        size_(other.size_),
        capacity_(other.capacity_) {
            other.data_ = nullptr;
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
        
        other.data_ = nullptr;
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
        return data_ ? data_ : "";
    }

    void reserve(std::size_t new_capacity) {
        if (capacity_ >= new_capacity) return;
        char* new_data = new char[new_capacity + 1];
        if (data_) {
            std::memcpy(new_data, data_, size_ + 1);
        } else {
            new_data[0] = '\0';
        }
        
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

    void append(const char* str) {
        if (str == nullptr) return;
        std::size_t append_size = std::strlen(str);
        std::size_t new_size = append_size + size_;

        if (str >= data_ && str <= data_ + size_) {
            char* str_buffer = new char[append_size + 1];
            std::memcpy(str_buffer, str, append_size + 1);

            ensure_capacity(new_size);
            std::memcpy(data_ + size_, str_buffer, append_size + 1);

            delete[] str_buffer;
        } else {
            ensure_capacity(new_size);
            std::memcpy(data_ + size_, str, append_size + 1);
        }
        size_ = new_size;
    }

    void append(const MyString& other) {
        if (this == &other) {
            std::size_t new_size = 2 * size_;
            ensure_capacity(new_size);
            std::memmove(data_ + size_, data_, size_);
            size_ = new_size;
            data_[size_] = '\0';
            return;
        } else {
            std::size_t new_size = size_ + other.size_;
            ensure_capacity(new_size);
            std::memcpy(data_ + size_, other.data_, other.size_ + 1);
            size_ = new_size;
        }
    }

    MyString& operator+=(const char* str) {
        append(str);
        return *this;
    }

    MyString& operator+=(const MyString& other) {
        append(other);
        return *this;
    }

    MyString& operator+=(char ch) {
        push_back(ch);
        return *this;
    }

    char& operator[](std::size_t i) {
        return data_[i];
    } 

    const char& operator[](std::size_t i) const {
        return data_[i];
    }

    char& at(std::size_t i) {
        if (i >= size_) {
            throw std::out_of_range("MyString::at out of range");
        }
        return data_[i];
    }

    const char& at(std::size_t i) const {
        if (i >= size_) {
            throw std::out_of_range("MyString::at out of range");
        }
        return data_[i];
    }

    bool empty() const {
        return size_ == 0;
    }

    void clear() {
        size_ = 0;
        if (data_) {
            data_[size_] = '\0';
        }
    }

    void pop_back() {
        if (size_ == 0) {
            throw std::out_of_range("MyString::pop_back on empty string");
        }
        --size_;
        data_[size_] = '\0';
    }

    char& back() {
        if (size_ == 0) {
            throw std::out_of_range("MyString::back on empty string");
        }
        return data_[size_ - 1];
    }

    const char& back() const {
        if (size_ == 0) {
            throw std::out_of_range("MyString::back on empty string");
        }
        return data_[size_ - 1];
    }

    void resize(std::size_t new_size, char ch) {
        if (new_size < size_) {
            size_ = new_size;
            data_[size_] = '\0';
            return;
        }
        if (new_size > size_) {
            ensure_capacity(new_size + 1);
            for (std::size_t i = size_; i < new_size; ++i) {
                data_[i] = ch;
            }
            size_ = new_size;
            data_[size_] = '\0';
        }
    }

    void resize(std::size_t new_size) {
        resize(new_size, '\0');
    }

    void swap(MyString& other) noexcept {
        char* tmp_data = data_;
        data_ = other.data_;
        other.data_ = tmp_data;

        std::size_t tmp_size = size_;
        size_ = other.size_;
        other.size_ = tmp_size;

        std::size_t tmp_capacity = capacity_;
        capacity_ = other.capacity_;
        other.capacity_ = tmp_capacity;
    }

    void insert(std::size_t pos, const char* str) {
        if (pos > size_) {
            throw std::out_of_range("MyString::insert position out of range");
        }

        if (str == nullptr || str[0] == '\0') return;

        std::size_t insert_len = std::strlen(str);
        ensure_capacity(insert_len + size_);
        
        if (data_ && str >= data_ && str <= data_ + size_ + 1) {
            char* temp = new char[insert_len + 1];
            std::memcpy(temp, str, insert_len + 1);

            std::memmove(data_ + pos + insert_len, data_ + pos, size_ - pos + 1);
            std::memcpy(data_ + pos, temp, insert_len);

            delete[] temp;
        } else {
            std::memmove(data_ + pos + insert_len, data_ + pos, size_ - pos + 1);
            std::memcpy(data_ + pos, str, insert_len);
        }

        size_ = size_ + insert_len;

    }

    void insert(std::size_t pos, const MyString& other) {
        insert(pos, other.c_str());
    }

    void insert(std::size_t pos, char ch) {
        if (pos > size_) {
            throw std::out_of_range("MyString::insert position out of range");
        }

        ensure_capacity(size_ + 1);
        std::memmove(data_ + pos + 1, data_ + pos, size_ - pos + 1);
        data_[pos] = ch;

        ++size_;
    }

    void erase(std::size_t pos, std::size_t count = 1) {
        if (pos > size_) {
            throw std::out_of_range("MyString::erase position out of range");
        }
        if (pos == size_ || count == 0) return;
        
        std::size_t erase_count = std::min(count, size_ - pos);
        std::memmove(data_ + pos, data_ + pos + erase_count, size_ - pos - erase_count + 1);
        
        size_ -= erase_count;

    }

};