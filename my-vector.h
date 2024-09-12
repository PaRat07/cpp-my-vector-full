#include <bits/stdc++.h>

template<class T>
void Destroy(T *from, T *to) {
    std::for_each(from, to, [](T &val) {
        (&val)->~T();
    });
}

template<class T>
void DestroyN(T *from, size_t n) {
    Destroy(from, from + n);
}

template<class T>
void Construct(T *from, T *to) {
    std::for_each(from, to, [](T &val) {
        new (&val) T();
    });
}

template<class T>
void ConstructN(T *from, size_t n) {
    Construct(from, from + n);
}


template<typename T, class Allocator = std::allocator<T>>
class MyVector {
public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    MyVector() noexcept(noexcept(Allocator())) = default;

    explicit MyVector(const Allocator &allocator)
            : allocator_(allocator) {
    }

    MyVector(size_type count,
             const T &value,
             const Allocator &alloc = Allocator())
            : allocator_(alloc), data_(count == 0 ? nullptr : allocator_.allocate(count)), size_(count),
              capacity_(count) {
        std::uninitialized_fill_n(data_, count, value);
    }

    explicit MyVector(size_type count,
                      Allocator alloc)
            : allocator_(alloc), data_(count == 0 ? nullptr : allocator_.allocate(count)), size_(count),
              capacity_(count) {
    }

    template<typename InputIt>
    MyVector(InputIt first, InputIt last,
             const Allocator &alloc = Allocator())
            : allocator_(alloc), size_(std::distance(first, last)), capacity_(size_),
              data_(size_ == 0 ? nullptr : allocator_.allocate(size_)) {
        std::uninitialized_copy(first, last, data_);
    }

    MyVector(const MyVector &other) {
        allocator_ = other.allocator_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = allocator_.allocate(capacity_);
        std::uninitialized_copy(other.begin(), other.end(), data_);
    }

    MyVector(MyVector &&other) {
        std::swap(allocator_, other.allocator_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(data_, other.data_);
    }

    MyVector(MyVector &&other,
             const Allocator &alloc) {
        allocator_ = alloc;
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(data_, other.data_);
    }

    MyVector(std::initializer_list<T> init,
             const Allocator &alloc = Allocator())
            : MyVector(init.begin(), init.end(), alloc) {
    }

    ~MyVector() {
        if (data_ != nullptr) {
            allocator_.deallocate(data_, capacity_);
        }
    }

    MyVector &operator=(MyVector &&other) {
        std::swap(allocator_, other.allocator_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(data_, other.data_);
    }

    MyVector &operator=(const MyVector &other) {
        *this = MyVector(other);
    }

    MyVector &operator=(std::initializer_list<value_type> ilist) {
        *this = MyVector(std::move(ilist));
    }

    void assign(size_type count, const T &value) {
        if (capacity_ >= count) {
            std::fill_n(data_, count, value);
            size_ = count;
        } else {
            *this = MyVector(count, value);
        }
    }

    template<class InputIt>
    void assign(InputIt first, InputIt last) {
        if (capacity_ >= std::distance(first, last)) {
            // TODO maybe move
            std::uninitialized_copy(first, last, data_);
            size_ = std::distance(first, last);
        } else {
            *this = MyVector(first, last);
        }
    }

    void assign(std::initializer_list<T> ilist) {
        if (capacity_ > ilist.size()) {
            std::uninitialized_copy(ilist.begin(), ilist.end(), data_);
            size_ = ilist.size();
        } else {
            *this = MyVector(std::move(ilist));
        }
    }

    allocator_type get_allocator() const {
        return allocator_;
    }

    reference at(size_t ind) {
        if (ind >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[ind];
    }

    const_reference at(size_t ind) const {
        if (ind >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[ind];
    }

    reference operator[](size_t ind) {
        return data_[ind];
    }

    const_reference operator[](size_t ind) const {
        return data_[ind];
    }

    reference front() {
        return *data_;
    }

    const_reference front() const {
        return *data_;
    }

    reference back() {
        return data_[size_ - 1];
    }

    const_reference back() const {
        return data_[size_ - 1];
    }

    pointer data() {
        return data_;
    }

    const_pointer data() const {
        return data_;
    }

    iterator begin() {
        return data_;
    }

    const_iterator begin() const {
        return data_;
    }

    iterator end() {
        return data_ + size_;
    }

    const_iterator end() const {
        return data_ + size_;
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    const_iterator cbegin() const {
        return begin();
    }

    const_iterator cend() const {
        return end();
    }

    const_reverse_iterator crbegin() const {
        return rbegin();
    }

    const_reverse_iterator crend() const {
        return rend();
    }

    bool empty() const {
        return size_ == 0;
    }

    size_t size() const {
        return size_;
    }

    size_t max_size() const {
        return allocator_.max_size();
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > max_size()) {
            using namespace std::string_literals;
            throw std::length_error("new_capacity in "s + __PRETTY_FUNCTION__ + "is bigger then max_size()"s);
        }
        if (new_capacity > capacity_) {
            T *new_data = allocator_.allocate(new_capacity);
            // TODO sometimes move
            std::uninitialized_copy_n(data_, size_, new_data);
            if (data_ != nullptr) {
                allocator_.deallocate(data_, capacity_);
            }
            data_ = new_data;
            capacity_ = new_capacity;
        }
    }

    size_t capacity() const {
        return capacity_;
    }

    void shrink_to_fit() {
        if (capacity_ > size_) {
            *this = MyVector(*this);
        }
    }

    void clear() {
        Destroy(begin(), end());
        size_ = 0;
    }

    iterator insert(const_iterator pos,
                    size_type count,
                    const T &value) {
        {
            size_t ind = pos - begin();
            reserve(size_ + count);
            pos = begin() + ind;
        }
        ConstructN(end(), count);
        std::move_backward(pos, end(), pos + count);
        std::fill_n(pos, count, value);
        return pos;
    }

    iterator insert(const_iterator pos, T &&value) {
        {
            size_t ind = pos - begin();
            reserve(size_ + 1);
            pos = begin() + ind;
        }
        ConstructN(end(), 1);
        std::move_backward(pos, end(), pos + 1);
        *pos = std::move(value);
        return pos;
    }

    iterator insert(const_iterator pos, const T &value) {
        return insert(pos, 1, value);
    }

    template<typename... Args_t>
    iterator emplace(const_iterator pos, Args_t &&... args) {
        if (pos == end()) {
            pos->T(std::forward<Args_t>(args)...);
            ++size_;
            return pos;
        }
        return insert(pos, T(std::forward<Args_t>(args)...));
    }

    iterator erase(const_iterator first, const_iterator last) {
        std::copy(std::make_move_iterator(last), std::make_move_iterator(end()), first);
        resize(size_ - (last - first));
    }

    iterator erase(const_iterator pos) {
        return erase(pos, pos + 1);
    }

    void resize(size_t new_size) {
        if (new_size < size_) {
            Destroy(begin() + (new_size - 1), end());
            size_ = new_size;
        } else {
            reserve(new_size);
            Construct(end(), begin() + new_size);
            size_ = new_size;
        }
    }

    void resize(size_t new_size, const value_type &val) {
        if (new_size < size_) {
            Destroy(begin() + (new_size - 1), end());
            size_ = new_size;
        } else {
            reserve(new_size);
            std::uninitialized_fill(end(), begin() + new_size, val);
            size_ = new_size;
        }
    }

    void push_back(T &&value) {
        insert(end(), std::move(value));
    }

    void push_back(const T &value) {
        insert(end(), value);
    }

    template<class... Args_t>
    void emplace_back(Args_t &&... args) {
        emplace(end(), std::forward<Args_t>(args)...);
    }

    void pop_back() {
        erase(end() - 1);
    }

    template<typename _T, typename _Alloc>
    friend void swap(MyVector<_T, _Alloc> &lhs, MyVector<_T, _Alloc> &rhs) {
        std::swap(lhs.allocator_, rhs.allocator_);
        std::swap(lhs.size_, rhs.size_);
        std::swap(lhs.capacity_, rhs.capacity_);
        std::swap(lhs.data_, rhs.data_);
    }

private:
    Allocator allocator_ = {};
    size_t size_ = 0;
    size_t capacity_ = 0;
    T *data_ = nullptr;
};

template<class T, class Alloc>
bool operator<(const MyVector<T, Alloc> &lhs,
               const MyVector<T, Alloc> &rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T, class Alloc>
bool operator>(const MyVector<T, Alloc> &lhs,
               const MyVector<T, Alloc> &rhs) {
    return rhs < lhs;
}

template<class T, class Alloc>
bool operator<=(const MyVector<T, Alloc> &lhs,
                const MyVector<T, Alloc> &rhs) {
    return !(lhs > rhs);
}

template<class T, class Alloc>
bool operator>=(const MyVector<T, Alloc> &lhs,
                const MyVector<T, Alloc> &rhs) {
    return !(lhs < rhs);
}

template<class T, class Alloc>
bool operator==(const MyVector<T, Alloc> &lhs,
                const MyVector<T, Alloc> &rhs) {
    return !(lhs < rhs) && !(lhs > rhs);
}

template<class T, class Alloc>
bool operator!=(const MyVector<T, Alloc> &lhs,
                const MyVector<T, Alloc> &rhs) {
    return !(lhs == rhs);
}
