#ifndef MYDEQUE_DEQUE_H
#define MYDEQUE_DEQUE_H

#include <cstdlib>
#include <cmath>
#include <iterator>


template<class T>
class Deque {
    T *data;
    size_t dataSize, _size;
    size_t start;
    static const size_t minSize = 14;
    static const size_t startModifier = 3;
    static constexpr const double shrinkKoef = 4;
    static constexpr const double shrinkDivisor = 2;
    static constexpr const double expandMultiplier = 2;

    void reallocate(size_t newDataSize) {
        if (newDataSize < minSize)
            newDataSize = minSize;
        T* newData = new T[newDataSize];
        for (size_t i = 0; i < _size; ++i) {
            newData[i] = data[(i + start) % dataSize];
        }
        delete[] data;
        data = newData;
        start = 0;
        dataSize = newDataSize;
    }

    void reallocateIfNeeded() {
        if (_size >= dataSize) {
            reallocate(round(expandMultiplier * dataSize));
        }
        if (_size > minSize && _size * shrinkKoef < dataSize) {
            reallocate(ceil(dataSize / shrinkDivisor) + 1);
        }
    }

public:
    Deque(): data(nullptr), dataSize(0), _size(0), start(0) {
        reallocate(minSize);
    }

    Deque(size_t cnt): data(nullptr), dataSize(0), _size(0), start(0) {
        reallocate(cnt * startModifier);
        _size = cnt;
        for (size_t i = 0; i < _size; ++i)
            data[i] = T();
    }

    void swap(Deque& other) {
        using std::swap;
        swap(data, other.data);
        swap(dataSize, other.dataSize);
        swap(_size, other._size);
        swap(start, other.start);
    }

    Deque& operator=(const Deque& other) {
        Deque temp = other;
        temp.swap(*this);
        return *this;
    }

    ~Deque() {
        delete[] data;
    }

    Deque(const Deque &d) {
        dataSize = d.dataSize;
        _size = d._size;
        start = d.start;
        data = new T[dataSize];
        for (size_t i = 0; i < _size; ++i) {
            data[(start + i) % dataSize] = d.data[(start + i) % dataSize];
        }
    }

    void push_front(const T &a) {
        start = (start + dataSize - 1) % dataSize;
        ++_size;
        data[start] = a;
        reallocateIfNeeded();
    }

    void push_back(const T &a) {
        data[(start + _size) % dataSize] = a;
        ++_size;
        reallocateIfNeeded();
    }

    void pop_back() {
        --_size;
        reallocateIfNeeded();
    }

    void pop_front() {
        start = (start + 1) % dataSize;
        --_size;
        reallocateIfNeeded();
    }

    size_t size() const {
        return _size;
    }

    bool empty() const {
        return !_size;
    }

    T& front() {
        return data[start];
    }

    const T& front() const {
        return data[start];
    }

    T& back() {
        return data[(start + size() - 1) % dataSize];
    }

    const T& back() const {
        return data[(start + size() - 1) % dataSize];
    }

    T& operator[](size_t i) {
        return data[(start + i) % dataSize];
    }

    const T& operator[](size_t i) const {
        return data[(start + i) % dataSize];
    }

protected:
    class BaseIterator: public std::iterator<
             std::random_access_iterator_tag,
             T,
             std::ptrdiff_t,
             T*,
             T&
    > {
    public:
        typedef typename std::iterator<std::random_access_iterator_tag, T, std::ptrdiff_t, T*, T&>::difference_type difference_type;
        typedef typename std::iterator<std::random_access_iterator_tag, T, std::ptrdiff_t, T*, T&>::value_type value_type;
        typedef typename std::iterator<std::random_access_iterator_tag, T, std::ptrdiff_t, T*, T&>::pointer pointer;
        typedef typename std::iterator<std::random_access_iterator_tag, T, std::ptrdiff_t, T*, T&>::reference reference;
        typedef typename std::iterator<std::random_access_iterator_tag, T, std::ptrdiff_t, T*, T&>::iterator_category iterator_category;

    public:
        size_t offset, dequeBufferSize, dequeSize, first;
        pointer dequeData;
    public:
        BaseIterator(): offset(0), first(0), dequeBufferSize(0), dequeSize(0), dequeData(nullptr) {}

        BaseIterator(const Deque &d , size_t position) {
            dequeData = d.data;
            dequeSize = d._size;
            dequeBufferSize = d.dataSize;
            first = d.start;
            offset = position;
        }

        BaseIterator(const BaseIterator &i) {
            offset = i.offset;
            dequeSize = i.dequeSize;
            dequeData = i.dequeData;
            dequeBufferSize = i.dequeBufferSize;
            first = i.first;
        }

        bool operator==(const BaseIterator &i) const {
            return dequeData == i.dequeData && i.offset == offset;
        }

        bool operator!=(const BaseIterator &i) const {
            return !(*this == i);
        }

        void advance(size_t i) {
            offset += i;
        }

        difference_type operator-(const BaseIterator &i) const {
            return (offset - i.offset);
        }

        bool operator<(const BaseIterator &other) const {
            return offset < other.offset;
        }

        bool operator>(const BaseIterator &other) const {
            return other < *this;
        }

        bool operator<=(const BaseIterator &other) const {
            return other == *this || other < *this;
        }

        bool operator>=(const BaseIterator &other) const {
            return other == *this || other > *this;
        }

        reference referenceAt(size_t pos) const {
            return dequeData[(offset + first + pos) % dequeBufferSize];
        }
    };

public:
    class iterator: public BaseIterator {
    public:
        typedef typename BaseIterator::difference_type difference_type;
        typedef typename BaseIterator::value_type value_type;
        typedef typename BaseIterator::reference reference;
        typedef typename BaseIterator::pointer pointer;

        typedef typename BaseIterator::iterator_category iterator_category;

        iterator(): BaseIterator() {}

        iterator(const Deque &d , size_t position): BaseIterator(d, position) {}

        iterator(const BaseIterator &i): BaseIterator(i) {}

        virtual iterator& operator++() {
            BaseIterator::advance(1);
            return *this;
        }

        virtual const iterator operator++(int) {
            iterator i = *this;
            ++(*this);
            return i;
        }

        virtual iterator& operator--() {
            BaseIterator::advance(-1);
            return *this;
        }

        virtual const iterator operator--(int) {
            iterator i = *this;
            --(*this);
            return i;
        }

        virtual iterator& operator+=(difference_type a) {
            BaseIterator::advance(a);
            return *this;
        }

        virtual iterator& operator-=(difference_type a) {
            return *this += -a;
        }

        reference operator*() const {
            return BaseIterator::referenceAt(0);
        }

        pointer operator->() const {
            return &(*this).operator*();
        }

        reference operator[](size_t i) const {
            return BaseIterator::referenceAt(i);
        }
    };

    class const_iterator: public BaseIterator {
    public:
        typedef typename BaseIterator::value_type value_type;
        typedef typename BaseIterator::iterator_category iterator_category;
        typedef const value_type& reference;
        typedef const value_type* pointer;
        typedef typename BaseIterator::difference_type difference_type;

        const_iterator(): BaseIterator() {}

        const_iterator(const Deque &d , size_t pos): BaseIterator(d, pos) {}

        const_iterator(const BaseIterator &i): BaseIterator(i) {}

        virtual const_iterator& operator++() {
            BaseIterator::advance(1);
            return *this;
        }

        virtual const const_iterator operator++(int) {
            const_iterator i = *this;
            ++(*this);
            return i;
        }

        virtual const_iterator& operator--() {
            BaseIterator::advance(-1);
            return *this;
        }

        virtual const const_iterator operator--(int) {
            const_iterator i = *this;
            --(*this);
            return i;
        }

        virtual const_iterator& operator+=(difference_type a) {
            BaseIterator::advance(a);
            return *this;
        }

        virtual const_iterator& operator-=(difference_type a) {
            return *this += -a;
        }

        reference operator*() const {
            return BaseIterator::referenceAt(0);
        }

        pointer operator->() const {
            return &operator*();
        }

        reference operator[](size_t i) const {
            return BaseIterator::referenceAt(i);
        }
    };

    iterator begin() {
        return iterator(*this, 0);
    }

    const_iterator cbegin() const {
        return const_iterator(*this, 0);
    }

    const_iterator begin() const {
        return cbegin();
    }

    iterator end() {
        return iterator(*this, _size);
    }

    const_iterator cend() const {
        return const_iterator(*this, _size);
    }

    const_iterator end() const {
        return cend();
    }

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator rbegin() const {
        return crbegin();
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(cbegin());
    }

    const_reverse_iterator rend() const {
        return crend();
    }

};

template<class Iterator>
const Iterator operator+(const Iterator &it, typename Iterator::difference_type a) {
    Iterator j = it;
    j += a;
    return j;
}

template<class Iterator>
const Iterator operator+(typename Iterator::difference_type a, const Iterator &it) {
    return (it + a);
}

template<class Iterator>
const Iterator operator-(const Iterator &it, typename Iterator::difference_type a) {
    return (it + -a);
}

#endif //MYDEQUE_DEQUE_H
