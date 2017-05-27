#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {

template<typename T>
class vector {
private:
    T *storage;
    int maxSize, nowSize;

    void doubleSpace() {
        if (maxSize == 0) maxSize = 10;
        maxSize <<= 1;
        T *tmp = storage;
        storage = (T*)::operator new(maxSize * sizeof(T));
        for (int i = 0; i < nowSize; ++i)
            storage[i] = tmp[i];
        ::operator delete(tmp);
    }
public:
	class const_iterator;
	class iterator {
        friend class vector<T>;
	private:
        vector<T> *ctn;
        int idx;
	public:
        iterator(vector<T> *p, int x): ctn(p), idx(x) {}
		iterator operator+(const int &n) const {
			return iterator(ctn, idx + n);
		}
		iterator operator-(const int &n) const {
			return iterator(ctn, idx - n);
		}
		int operator-(const iterator &rhs) const {
            if (ctn != rhs.ctn) throw invalid_iterator();
            return idx - rhs.idx;
		}
		iterator operator+=(const int &n) {
            idx += n;
            return *this;
		}
		iterator operator-=(const int &n) {
			idx -= n;
            return *this;
		}
		iterator operator++(int) {
            iterator tmp = *this;
            ++idx;
            return tmp;
		}
		iterator& operator++() {
            ++idx;
            return *this;
		}
		iterator operator--(int) {
            iterator tmp = *this;
            --idx;
            return tmp;
		}
		iterator& operator--() {
            --idx;
            return *this;
		}
        T& operator*() const{
            return ctn -> storage[idx];
		}
		bool operator==(const iterator &rhs) const {
            return (ctn == rhs.ctn) && (idx == rhs.idx);
		}
		bool operator==(const const_iterator &rhs) const {
            return (ctn == rhs.ctn) && (idx == rhs.idx);
		}
		bool operator!=(const iterator &rhs) const {
            return (ctn != rhs.ctn) || (idx != rhs.idx);
        }
		bool operator!=(const const_iterator &rhs) const {
            return (ctn != rhs.ctn) || (idx != rhs.idx);
		}
	};
	class const_iterator {
        friend class vector<T>;
    private:
        const vector<T> *ctn;
        int idx;
	public:
        const_iterator(const vector<T> *p, int x): ctn(p), idx(x) {}
		const_iterator operator+(const int &n) const {
			return const_iterator(ctn, idx + n);
		}
		const_iterator operator-(const int &n) const {
			return const_iterator(ctn, idx - n);
		}
		int operator-(const const_iterator &rhs) const {
            if (ctn != rhs.ctn) throw invalid_iterator();
            return idx - rhs.idx;
		}
		const_iterator operator+=(const int &n) {
            idx += n;
            return *this;
		}
		const_iterator operator-=(const int &n) {
			idx -= n;
            return *this;
		}
		const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++idx;
            return tmp;
		}
		const_iterator& operator++() {
            ++idx;
            return *this;
		}
		const_iterator operator--(int) {
            const_iterator tmp = *this;
            --idx;
            return tmp;
		}
		const_iterator& operator--() {
            --idx;
            return *this;
		}
        const T& operator*() const{
            return ctn -> storage[idx];
		}
		bool operator==(const iterator &rhs) const {
            return (ctn == rhs.ctn) && (idx == rhs.idx);
		}
		bool operator==(const const_iterator &rhs) const {
            return (ctn == rhs.ctn) && (idx == rhs.idx);
		}
		bool operator!=(const iterator &rhs) const {
            return (ctn != rhs.ctn) || (idx != rhs.idx);
        }
		bool operator!=(const const_iterator &rhs) const {
            return (ctn != rhs.ctn) || (idx != rhs.idx);
		}
	};
	vector(): nowSize(0), maxSize(10) {
        storage = (T*)::operator new(maxSize * sizeof(T));
	}
	vector(const vector &other): nowSize(other.nowSize), maxSize(other.maxSize) {
        storage = (T*)::operator new(maxSize * sizeof(T));
        for (int i = 0; i < nowSize; ++i)
            storage[i] = other.storage[i];
	}
    template<class OVector>
    vector(const OVector &other): nowSize(other.size()), maxSize(other.capacity()) {
        storage = (T*)::operator new(maxSize * sizeof(T));
        for (int i = 0; i < nowSize; ++i)
            storage[i] = other[i];
    }
	~vector() {
	    for (int i = 0; i < nowSize; ++i)
            storage[i].~T();
        if (storage) ::operator delete(storage);
	}
	vector &operator=(const vector &other) {
        if (this == &other) return *this;
        if (storage) ::operator delete(storage);
        nowSize = other.nowSize;
        maxSize = other.maxSize;
        storage = (T*)::operator new(maxSize * sizeof(T));
        for (int i = 0; i < nowSize; ++i)
            storage[i] = other.storage[i];
        return *this;
	}
	T & at(const size_t &pos) {
        if (pos < 0 || pos >= nowSize) throw index_out_of_bound();
        return storage[pos];
	}
	const T & at(const size_t &pos) const {
        if (pos < 0 || pos >= nowSize) throw index_out_of_bound();
        return storage[pos];
    }
	T & operator[](const size_t &pos) {
        if (pos < 0 || pos >= nowSize) throw index_out_of_bound();
        return storage[pos];
	}
	const T & operator[](const size_t &pos) const {
        if (pos < 0 || pos >= nowSize) throw index_out_of_bound();
        return storage[pos];
	}
	const T & front() const {
        if (nowSize == 0) throw container_is_empty();
        return storage[0];
	}
	const T & back() const {
        if (nowSize == 0) throw container_is_empty();
        return storage[nowSize - 1];
	}
	iterator begin() {
        return iterator(this, 0);
	}
	const_iterator cbegin() const {
        return const_iterator(this, 0);
	}
	iterator end() {
        return iterator(this, nowSize);
	}
	const_iterator cend() const {
        return const_iterator(this, nowSize);
	}
	bool empty() const {
        return nowSize == 0;
	}
	size_t size() const {
        return nowSize;
	}
	size_t capacity() const {
        return maxSize;
	}
	void clear() {
        for (int i = 0; i < nowSize; ++i)
            storage[i].~T();
        if (storage) ::operator delete(storage);
        nowSize = 0;
        maxSize = 10;
        storage = (T*)::operator new(maxSize * sizeof(T));
	}
	iterator insert(iterator pos, const T &value) {
        if (nowSize == maxSize) doubleSpace();
        for (int i = nowSize; i > pos.idx; --i)
            storage[i] = storage[i - 1];
        ++nowSize;
        ::new (&storage[pos.idx]) T(value);
        return iterator(this, pos.idx);
	}
	iterator insert(const size_t &ind, const T &value) {
        if (ind > nowSize) throw index_out_of_bound();
        if (nowSize == maxSize) doubleSpace();
        for (int i = nowSize; i > ind; --i)
            storage[i] = storage[i - 1];
        ++nowSize;
        ::new (&storage[ind]) T(value);
        return iterator(this, ind);
	}
	iterator erase(iterator pos) {
        --nowSize;
        for (int i = pos.idx; i < nowSize; ++i)
            storage[i] = storage[i + 1];
        storage[nowSize].~T();
        return pos;
	}
	iterator erase(const size_t &ind) {
        if (ind >= nowSize) throw index_out_of_bound();
        --nowSize;
        for (int i = ind; i < nowSize; ++i)
            storage[i] = storage[i + 1];
        storage[nowSize].~T();
        return iterator(this, ind);
	}
	void push_back(const T &value) {
        if (nowSize == maxSize) doubleSpace();
        ::new (&storage[nowSize]) T(value);
        ++nowSize;
	}
	void pop_back() {
        if (nowSize == 0) throw container_is_empty();
        --nowSize;
        storage[nowSize].~T();
	}
};


}

#endif
