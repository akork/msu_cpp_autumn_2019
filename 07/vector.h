template <class T> class Iterator
{
  public:
    using difference_type = ptrdiff_t;
    using value_type = T;
    using reference = T &;
    using pointer = T *;
    using iterator_category = std::random_access_iterator_tag;
    using const_pointer = const pointer;
    using const_reference = const T &;
    using size_type = std::size_t;

    Iterator() {}
    Iterator(pointer p) : p_(p) {}

    pointer operator->() { return p_; }
    const_pointer operator->() const { return p_; }

    reference operator*() { return *p_; }
    const_reference operator*() const { return *p_; }

    Iterator &operator++()
    {
        ++p_;
        return *this;
    }
    Iterator operator++(int)
    {
        Iterator i(*this);
        ++p_;
        return i;
    }
    Iterator &operator--()
    {
        --p_;
        return *this;
    }
    Iterator operator--(int)
    {
        Iterator i(*this);
        --p_;
        return i;
    }

    Iterator &operator+=(size_type n)
    {
        p_ += n;
        return *this;
    }

    Iterator &operator-=(size_type n)
    {
        p_ -= n;
        return *this;
    }

    reference operator[](size_type n) { return *(p_ + n); }
    const_reference operator[](size_type n) const { return *(p_ + n); }

    friend Iterator operator+(const Iterator &it, size_type n)
    {
        return Iterator(it) += n;
    }
    friend Iterator operator+(size_type n, const Iterator &it)
    {
        return Iterator(it) += n;
    }
    friend Iterator operator-(const Iterator &it, size_type n)
    {
        return Iterator(it) -= n;
    }
    friend difference_type operator-(const Iterator &l, const Iterator &r)
    {
        return l.diff(r);
    }

    friend bool operator<(const Iterator &l, const Iterator &r)
    {
        return l.less(r);
    }
    friend bool operator>(const Iterator &l, const Iterator &r)
    {
        return r.less(l);
    }
    friend bool operator<=(const Iterator &l, const Iterator &r)
    {
        return l.less(r) || !l.not_equal(r);
    }
    friend bool operator>=(const Iterator &l, const Iterator &r)
    {
        return r.less(l) || !r.not_equal(l);
    }
    friend bool operator==(const Iterator &l, const Iterator &r)
    {
        return !l.not_equal(r);
    }
    friend bool operator!=(const Iterator &l, const Iterator &r)
    {
        return l.not_equal(r);
    }

    void swap(Iterator &other) { std::swap(p_, other.p_); }

    friend void swap(Iterator &l, Iterator &r) { l.swap(r); }

  private:
    bool not_equal(const Iterator &other) const { return p_ != other.p_; }
    bool less(const Iterator &other) const { return p_ < other.p_; }

    difference_type diff(const Iterator &other) const { return p_ - other.p_; }

  private:
    pointer p_ = nullptr;
};

template <class T> class Allocator
{
  public:
    using difference_type = ptrdiff_t;
    using value_type = T;
    using reference = T &;
    using pointer = T *;
    using const_pointer = const pointer;
    using const_reference = const T &;
    using size_type = std::size_t;

    template <class U> struct rebind {
        typedef Allocator<U> other;
    };

    Allocator() {}
    Allocator(const Allocator &) {}
    template <class U> Allocator(const Allocator<U> &) {}
    ~Allocator() {}

    T *allocate(size_type n, const void *hint = 0)
    {
        if (n > max_size())
            throw std::bad_alloc();
        return (T *)::operator new[](n * sizeof(T));
    }
    void deallocate(T *p, size_type n) { ::operator delete[](p); }

    size_type max_size() const
    {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }
};

template <class T, class Allocator = Allocator<T>> class Vector
{
  public:
    using iterator = Iterator<T>;
    using difference_type = ptrdiff_t;
    using value_type = T;
    using reference = T &;
    using pointer = T *;
    using const_pointer = const pointer;
    using const_reference = const T &;
    using size_type = std::size_t;

    iterator begin() noexcept { return iterator(begin_); }

    iterator end() noexcept { return iterator(end_); }

    std::reverse_iterator<iterator> rbegin() noexcept
    {
        return std::reverse_iterator<iterator>(iterator(end_));
    }
	
    std::reverse_iterator<iterator> rend() noexcept
    {
        return std::reverse_iterator<iterator>(iterator(begin_));
    }

    explicit Vector(const Allocator &alloc = Allocator()) : alloc_(alloc) {}

    ~Vector()
    {
        for (pointer p = begin_; p != end_; ++p)
            p->~T();
        alloc_.deallocate(begin_, capacity());
    }

    void clear()
    {
        for (pointer p = begin_; p != end_; ++p)
            p->~T();
        end_ = begin_;
    }

    void swap(Vector &v)
    {
        std::swap(v.begin_, begin_);
        std::swap(v.end_, end_);
        std::swap(v.fringe_, fringe_);
        std::swap(v.alloc_, alloc_);
    }

    void reserve(size_t n)
    {
        if (capacity() >= n)
            return;
        if (alloc_.max_size() <= n)
            throw std::bad_alloc();

        Vector v(alloc_);
        v.end_ = v.begin_ = alloc_.allocate(n);
        v.fringe_ = v.begin_ + n;

        for (pointer p = begin_; p != end_; ++p, ++v.end_)
            new ((void *)v.end_) T(*p);
        swap(v);
    }

    void resize(size_t n)
    {
        if (n <= size()) {
            while (n < size()) {
                end_->~T();
                --end_;
            }
            return;
        }
        if (n <= capacity()) {
            while (end_ < begin_ + n) {
                new ((void *)end_) T();
                ++end_;
            }
            return;
        }

        Vector v(alloc_);
        v.end_ = v.begin_ = alloc_.allocate(n);
        v.fringe_ = v.begin_ + n;

        for (pointer p = begin_; p != end_; ++p, ++v.end_)
            new ((void *)v.end_) T(*p);
        v.resize(n);
        swap(v);
    }

    reference operator[](size_t n) { return *(begin_ + n); }
    const_reference operator[](size_t n) const { return *(begin_ + n); }

    void push_back(const T &c)
    {
        const size_type n = size() + 1;

        if (n <= capacity()) {
            new ((void *)end_) T(c);
            ++end_;
            return;
        }

        if (alloc_.max_size() <= n)
            throw std::bad_alloc();

        Vector v;

		size_type new_capacity = capacity_factor * n;
        v.end_ = v.begin_ = alloc_.allocate(new_capacity);
        v.fringe_ = v.begin_ + new_capacity;

        for (pointer p = begin_; p != end_; ++p, ++v.end_)
            new ((void *)v.end_) T(*p);
        v.push_back(c);

        swap(v);
    }

    void pop_back() { resize(size() - 1); }

    bool empty() const noexcept { return begin_ == end_; }

    size_t size() const noexcept { return end_ - begin_; }

    size_t max_size() const noexcept { return alloc_.max_size(); }

    size_t capacity() const noexcept { return fringe_ - begin_; }

  private:
    const size_t capacity_factor = 2;
    T *begin_ = nullptr;
    T *end_ = nullptr;
    T *fringe_ = nullptr;
    Allocator alloc_;
};
