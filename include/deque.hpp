#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu {

template<class T>
class deque {
private:
    static const int maxBlockSize = 1000;
    struct ListBlock {
        size_t sizeB;
        T* data[maxBlockSize];
        ListBlock *prev, *next;
        ListBlock(): sizeB(0), prev(NULL), next(NULL) {
            for (int i = 0; i < maxBlockSize; ++i)
                data[i] = NULL;
        }
        ListBlock(const T &v): sizeB(1), prev(NULL), next(NULL) {
            data[0] = new T(v);
            for (int i = 1; i < maxBlockSize; ++i)
                data[i] = NULL;
        }
        ListBlock(const ListBlock &other): sizeB(other.sizeB), prev(NULL), next(NULL) {
            for (int i = 0; i < sizeB; ++i)
                data[i] = new T(*(other.data[i]));
            for (int i = sizeB; i < maxBlockSize; ++i)
                data[i] = NULL;
        }
        ~ListBlock() {
            for (int i = 0; i < sizeB; ++i)
                delete data[i];
        }
    };
    size_t sizeD;
    ListBlock *first, *last, pastTheEnd;

public:
	class const_iterator;
	class iterator {
        friend deque;
	private:
        deque *ctn;
        ListBlock *p;
        size_t idx;
        iterator(deque *_ctn, ListBlock *_p, size_t _idx):
                ctn(_ctn), p(_p), idx(_idx) {}
	public:
        iterator(): ctn(NULL), p(NULL), idx(0) {}
        iterator(const iterator &other):
            ctn(other.ctn), p(other.p), idx(other.idx) {}
		iterator operator+(const int &n) const {
		    if (n < 0) return operator-(-n);
            ListBlock *_p = p;
            size_t _idx = idx;
            int _n = n;
            while (_idx + _n >= _p->sizeB && _p != &(ctn->pastTheEnd)) {
                _n -= _p->sizeB - _idx;
                _p = _p->next;
                _idx = 0;
            }
            return iterator(ctn, _p, _idx + _n);
		}
		iterator operator-(const int &n) const {
            if (n < 0) return operator+(-n);
            ListBlock *_p = p;
            size_t _idx = idx;
            int _n = n;
            while (_idx < _n) {
                _n -= _idx + 1;
                _p = _p->prev;
                _idx = _p->sizeB - 1;
            }
            return iterator(ctn, _p, _idx - _n);
		}
		int operator-(const iterator &rhs) const {
            if (ctn != rhs.ctn) throw invalid_iterator();
            int order1, order2;
            ListBlock *_p;
            order1 = idx;
            for (_p = p->prev; _p; _p = _p->prev)
                order1 += _p->sizeB;
            order2 = rhs.idx;
            for (_p = (rhs.p)->prev; _p; _p = _p->prev)
                order2 += _p->sizeB;
            return order1 - order2;
		}
		iterator operator+=(const int &n) {
		    if (n < 0) return operator-=(-n);
			ListBlock *_p = p;
            size_t _idx = idx;
            int _n = n;
            while (_idx + _n >= _p->sizeB && _p != &(ctn->pastTheEnd)) {
                _n -= _p->sizeB - _idx;
                _p = _p->next;
                _idx = 0;
            }
            p = _p;
            idx = _idx + _n;
            return *this;
		}
		iterator operator-=(const int &n) {
		    if (n < 0) return operator+=(-n);
			ListBlock *_p = p;
            size_t _idx = idx;
            int _n = n;
            while (_idx < _n) {
                _n -= _idx + 1;
                _p = _p->prev;
                _idx = _p->sizeB - 1;
            }
            p = _p;
            idx = _idx - _n;
            return *this;
        }
		iterator operator++(int) {
		    if (operator==(ctn->end())) throw invalid_iterator();
            iterator ret(*this);
            if (idx + 1 < p->sizeB) ++idx;
            else {
                p = p->next;
                idx = 0;
            }
            return ret;
		}
		iterator& operator++() {
		    if (operator==(ctn->end())) throw invalid_iterator();
            if (idx + 1 < p->sizeB) ++idx;
            else {
                p = p->next;
                idx = 0;
            }
            return *this;
		}
		iterator operator--(int) {
		    if (operator==(ctn->begin())) throw invalid_iterator();
            iterator ret(*this);
            if (idx > 0) --idx;
            else {
                p = p->prev;
                idx = p->sizeB - 1;
            }
            return ret;
		}
		iterator& operator--() {
		    if (operator==(ctn->begin())) throw invalid_iterator();
            if (idx > 0) --idx;
            else {
                p = p->prev;
                idx = p->sizeB - 1;
            }
            return *this;
        }
		T& operator*() const {
		    if (p->data[idx] == NULL) throw invalid_iterator();
            return *(p->data[idx]);
		}
		T* operator->() const noexcept {
            return p->data[idx];
		}
		bool operator==(const iterator &rhs) const {
            return (ctn == rhs.ctn) && (p == rhs.p) && (idx == rhs.idx);
		}
		bool operator==(const const_iterator &rhs) const {
            return (ctn == rhs.ctn) && (p == rhs.p) && (idx == rhs.idx);
		}
		bool operator!=(const iterator &rhs) const {
            return (ctn != rhs.ctn) || (p != rhs.p) || (idx != rhs.idx);
		}
		bool operator!=(const const_iterator &rhs) const {
		    return (ctn != rhs.ctn) || (p != rhs.p) || (idx != rhs.idx);
		}
	};
	class const_iterator {
        friend deque;
    private:
        const deque *ctn;
        const ListBlock *p;
        size_t idx;
        const_iterator(const deque *_ctn, const ListBlock *_p, size_t _idx):
            ctn(_ctn), p(_p), idx(_idx) {}
    public:
        const_iterator(): ctn(NULL), p(NULL), idx(0) {}
        const_iterator(const const_iterator &other):
            ctn(other.ctn), p(other.p), idx(other.idx) {}
        const_iterator(const iterator &other):
            ctn(other.ctn), p(other.p), idx(other.idx) {}
        const_iterator operator+(const int &n) const {
            if (n < 0) return operator-(-n);
            const ListBlock *_p = p;
            size_t _idx = idx;
            int _n = n;
            while (_idx + _n >= _p->sizeB && _p != &(ctn->pastTheEnd)) {
                _n -= _p->sizeB - _idx;
                _p = _p->next;
                _idx = 0;
            }
            return const_iterator(ctn, _p, _idx + _n);
		}
		const_iterator operator-(const int &n) const {
            if (n < 0) return operator+(-n);
            const ListBlock *_p = p;
            size_t _idx = idx;
            int _n = n;
            while (_idx < _n) {
                _n -= _idx + 1;
                _p = _p->prev;
                _idx = _p->sizeB - 1;
            }
            return const_iterator(ctn, _p, _idx - _n);
		}
		int operator-(const const_iterator &rhs) const {
            if (ctn != rhs.ctn) throw invalid_iterator();
            int order1, order2;
            const ListBlock *_p;
            order1 = idx;
            for (_p = p->prev; _p; _p = _p->prev)
                order1 += _p->sizeB;
            order2 = rhs.idx;
            for (_p = (rhs.p)->prev; _p; _p = _p->prev)
                order2 += _p->sizeB;
            return order1 - order2;
		}
		const_iterator operator+=(const int &n) {
		    if (n < 0) return operator-=(-n);
			const ListBlock *_p = p;
            size_t _idx = idx;
            int _n = n;
            while (_idx + _n >= _p->sizeB && _p != &(ctn->pastTheEnd)) {
                _n -= _p->sizeB - _idx;
                _p = _p->next;
                _idx = 0;
            }
            p = _p;
            idx = _idx + _n;
            return *this;
		}
		const_iterator operator-=(const int &n) {
		    if (n < 0) return operator+=(-n);
			const ListBlock *_p = p;
            size_t _idx = idx;
            int _n = n;
            while (_idx < _n) {
                _n -= _idx + 1;
                _p = _p->prev;
                _idx = _p->sizeB - 1;
            }
            p = _p;
            idx = _idx - _n;
            return *this;
        }
		const_iterator operator++(int) {
		    if (operator==(ctn->cend())) throw invalid_iterator();
            const_iterator ret(*this);
            if (idx + 1 < p->sizeB) ++idx;
            else {
                p = p->next;
                idx = 0;
            }
            return ret;
		}
		const_iterator& operator++() {
		    if (operator==(ctn->cend())) throw invalid_iterator();
            if (idx + 1 < p->sizeB) ++idx;
            else {
                p = p->next;
                idx = 0;
            }
            return *this;
		}
		const_iterator operator--(int) {
		    if (operator==(ctn->cbegin())) throw invalid_iterator();
            const_iterator ret(*this);
            if (idx > 0) --idx;
            else {
                p = p->prev;
                idx = p->sizeB - 1;
            }
            return ret;
		}
		const_iterator& operator--() {
		    if (operator==(ctn->cbegin())) throw invalid_iterator();
            if (idx > 0) --idx;
            else {
                p = p->prev;
                idx = p->sizeB - 1;
            }
            return *this;
        }
		const T& operator*() const {
		    if (p->data[idx] == NULL) throw invalid_iterator();
            return *(p->data[idx]);
		}
		const T* operator->() const noexcept {
            return p->data[idx];
		}
		bool operator==(const iterator &rhs) const {
            return (ctn == rhs.ctn) && (p == rhs.p) && (idx == rhs.idx);
		}
		bool operator==(const const_iterator &rhs) const {
            return (ctn == rhs.ctn) && (p == rhs.p) && (idx == rhs.idx);
		}
		bool operator!=(const iterator &rhs) const {
            return (ctn != rhs.ctn) || (p != rhs.p) || (idx != rhs.idx);
		}
		bool operator!=(const const_iterator &rhs) const {
		    return (ctn != rhs.ctn) || (p != rhs.p) || (idx != rhs.idx);
		}
	};
	deque(): sizeD(0), first(NULL), last(NULL) {}
	deque(const deque &other): sizeD(other.sizeD) {
        if (other.sizeD == 0) {
            first = last = NULL;
            return;
        }
        ListBlock *newBlock, *prevBlock;
        prevBlock = NULL;
        const ListBlock *origBlock = other.first;
        while (true) {
            newBlock = new ListBlock(*origBlock);
            newBlock->prev = prevBlock;
            if (origBlock == other.first) first = newBlock;
            if (prevBlock) prevBlock->next = newBlock;
            prevBlock = newBlock;
            if (origBlock == other.last) {
                last = newBlock;
                break;
            }
            origBlock = origBlock->next;
        }
        last->next = &pastTheEnd;
        pastTheEnd.prev = last;
    }
	~deque() {
        clear();
	}
	deque &operator=(const deque &other) {
        if (this == &other) return *this;
        clear();
        sizeD = other.sizeD;
        if (other.sizeD == 0) {
            first = last = NULL;
            return *this;
        }
        ListBlock *newBlock, *prevBlock;
        prevBlock = NULL;
        const ListBlock *origBlock = other.first;
        while (true) {
            newBlock = new ListBlock(*origBlock);
            newBlock->prev = prevBlock;
            if (origBlock == other.first) first = newBlock;
            if (prevBlock) prevBlock->next = newBlock;
            prevBlock = newBlock;
            if (origBlock == other.last) {
                last = newBlock;
                break;
            }
            origBlock = origBlock->next;
        }
        last->next = &pastTheEnd;
        pastTheEnd.prev = last;
	    return *this;
    }
	T & at(const size_t &posOrig) {
	    size_t pos = posOrig;
        if (pos < 0 || pos >= sizeD) throw index_out_of_bound();
        ListBlock *nowBlock = first;
        while (pos >= nowBlock->sizeB) {
            pos -= nowBlock->sizeB;
            nowBlock = nowBlock->next;
        }
        return *(nowBlock->data[pos]);
	}
	const T & at(const size_t &posOrig) const {
	    size_t pos = posOrig;
        if (pos < 0 || pos >= sizeD) throw index_out_of_bound();
        const ListBlock *nowBlock = first;
        while (pos >= nowBlock->sizeB) {
            pos -= nowBlock->sizeB;
            nowBlock = nowBlock->next;
        }
        return *(nowBlock->data[pos]);
	}
	T & operator[](const size_t &posOrig) {
	    size_t pos = posOrig;
        if (pos < 0 || pos >= sizeD) throw index_out_of_bound();
        int idx = pos;
        ListBlock *nowBlock = first;
        while (idx >= nowBlock->sizeB) {
            idx -= nowBlock->sizeB;
            nowBlock = nowBlock->next;
        }
        return *(nowBlock->data[idx]);
	}
	const T & operator[](const size_t &posOrig) const {
	    size_t pos = posOrig;
        if (pos < 0 || pos >= sizeD) throw index_out_of_bound();
        const ListBlock *nowBlock = first;
        while (pos >= nowBlock->sizeB) {
            pos -= nowBlock->sizeB;
            nowBlock = nowBlock->next;
        }
        return *(nowBlock->data[pos]);
	}
	const T & front() const {
        if (sizeD == 0) throw container_is_empty();
        return *(first->data[0]);
	}
	const T & back() const {
        if (sizeD == 0) throw container_is_empty();
        return *(last->data[last->sizeB - 1]);
	}
	iterator begin() {
        if (sizeD == 0) return iterator(this, &pastTheEnd, 0);
        return iterator(this, first, 0);
	}
	const_iterator cbegin() const {
        if (sizeD == 0) return const_iterator(this, &pastTheEnd, 0);
        return const_iterator(this, first, 0);
	}
	iterator end() {
        return iterator(this, &pastTheEnd, 0);
	}
	const_iterator cend() const {
        return const_iterator(this, &pastTheEnd, 0);
	}
	bool empty() const {
        return sizeD == 0;
	}
	size_t size() const {
        return sizeD;
	}
	void clear() {
        if (sizeD == 0) return;
        ListBlock *nowBlock = first, *tmp;
        while (nowBlock != &pastTheEnd) {
            tmp = nowBlock;
            nowBlock = nowBlock->next;
            delete tmp;
        }
        pastTheEnd.prev = NULL;
        sizeD = 0;
	}
private:
    int GetRank(const ListBlock *p, int idx) const {
        int ret = idx;
        while (p != first) {
            p = p->prev;
            ret += p->sizeB;
        }
        return ret;
    }
    iterator GetByRank(int k) {
        ListBlock *nowBlock = first;
        while (k >= nowBlock->sizeB && nowBlock != &pastTheEnd) {
            k -= nowBlock->sizeB;
            nowBlock = nowBlock->next;
        }
        return iterator(this, nowBlock, k);
    }
    bool Mergeable(ListBlock *lBlock, ListBlock *rBlock) {
        if (lBlock == NULL || rBlock == NULL || rBlock == &pastTheEnd) return false;
        return lBlock->sizeB + rBlock->sizeB <= maxBlockSize;
    }
    void MergeBlock(ListBlock *lBlock, ListBlock *rBlock) {
        for (int i = 0; i < rBlock->sizeB; ++i)
            lBlock->data[lBlock->sizeB + i] = new T(*(rBlock->data[i]));
        lBlock->sizeB += rBlock->sizeB;
        if (rBlock == last) last = lBlock;
        lBlock->next = rBlock->next;
        if (lBlock->next) lBlock->next->prev = lBlock;
        delete rBlock;
    }
    ListBlock* SplitBlock(ListBlock *block, int idx) {
        ListBlock *newBlock = new ListBlock;
        newBlock->sizeB = block->sizeB - idx;
        for (int i = 0; i < newBlock->sizeB; ++i)
            newBlock->data[i] = new T(*(block->data[idx + i]));
        for (int i = idx; i < block->sizeB; ++i) {
            delete block->data[i];
            block->data[i] = NULL;
        }
        block->sizeB -= newBlock->sizeB;
        if (block == last) last = newBlock;
        newBlock->next = block->next;
        if (newBlock->next) newBlock->next->prev = newBlock;
        newBlock->prev = block;
        block->next = newBlock;
        return block;
    }
    void DeleteElement(ListBlock *block, int idx) {
        delete block->data[idx];
        for (int i = idx; i < block->sizeB - 1; ++i)
            block->data[i] = block->data[i + 1];
        block->data[block->sizeB - 1] = NULL;
        --(block->sizeB);
    }
    void InsertElement(ListBlock *block, int idx, const T &value) {
        ++(block->sizeB);
        for (int i = block->sizeB - 1; i > idx; --i)
            block->data[i] = block->data[i - 1];
        block->data[idx] = new T(value);
    }
public:
	iterator insert(iterator pos, const T &value) {
        if (pos.ctn != this || pos.p == NULL) throw invalid_iterator();
        ++sizeD;
        if (sizeD == 1) {
            ListBlock *newBlock = new ListBlock(value);
            first = last = newBlock;
            newBlock->prev = NULL;
            newBlock->next = &pastTheEnd;
            pastTheEnd.prev = newBlock;
            return begin();
        }
        if (pos.p != &pastTheEnd && (pos.p)->sizeB + 1 <= maxBlockSize) {
            InsertElement(pos.p, pos.idx, value);
            return pos;
        }
        int rankNew = GetRank(pos.p, pos.idx);
        ListBlock *newBlock = new ListBlock(value);
        ListBlock *prevBlock, *nextBlock;
        if (pos.idx == 0) {
            prevBlock = (pos.p)->prev;
            nextBlock = pos.p;
        }
        else {
            prevBlock = SplitBlock(pos.p, pos.idx);
            nextBlock = prevBlock->next;
        }
        if (prevBlock == NULL) {
            first = newBlock;
            newBlock->prev = NULL;
        }
        else {
            newBlock->prev = prevBlock;
            prevBlock->next = newBlock;
        }
        if (nextBlock == &pastTheEnd) last = newBlock;
        newBlock->next = nextBlock;
        nextBlock->prev = newBlock;
        if (Mergeable(prevBlock, newBlock)) MergeBlock(prevBlock, newBlock);
        if (Mergeable(nextBlock->prev, nextBlock)) MergeBlock(nextBlock->prev, nextBlock);
        return GetByRank(rankNew);
    }
	iterator erase(iterator pos) {
        if (sizeD == 0) throw container_is_empty();
        if (pos.ctn != this || pos.p == NULL || pos.p->sizeB == 0) throw invalid_iterator();
        --sizeD;
        if (sizeD == 0) {
            delete first;
            first = last = NULL;
            pastTheEnd.prev = NULL;
            return end();
        }
        int rankNew = GetRank(pos.p, pos.idx);
        ListBlock *prevBlock, *nextBlock;
        if (pos.p->sizeB == 1) {
            prevBlock = pos.p->prev;
            nextBlock = pos.p->next;
            if (pos.p == first) first = nextBlock;
            if (pos.p == last) last = prevBlock;
            if (prevBlock) prevBlock->next = nextBlock;
            if (nextBlock) nextBlock->prev = prevBlock;
            delete pos.p;
            if (Mergeable(prevBlock, nextBlock)) MergeBlock(prevBlock, nextBlock);
        }
        else {
            prevBlock = pos.p->prev;
            nextBlock = pos.p->next;
            DeleteElement(pos.p, pos.idx);
            if (Mergeable(prevBlock, pos.p)) MergeBlock(prevBlock, pos.p);
            else if (Mergeable(pos.p, nextBlock)) MergeBlock(pos.p, nextBlock);
        }
        return GetByRank(rankNew);
	}
	void push_back(const T &value) {
	    ++sizeD;
	    if (sizeD == 1) {
            ListBlock *newBlock = new ListBlock(value);
            first = last = newBlock;
            newBlock->next = &pastTheEnd;
            pastTheEnd.prev = newBlock;
            return;
	    }
	    if (last->sizeB + 1 <= maxBlockSize) InsertElement(last, last->sizeB, value);
	    else {
            ListBlock *newBlock = new ListBlock(value);
            newBlock->prev = last;
            newBlock->next = &pastTheEnd;
            pastTheEnd.prev = newBlock;
            last->next = newBlock;
            last = newBlock;
	    }
	}
	void pop_back() {
        if (sizeD == 0) throw container_is_empty();
        --sizeD;
        if (sizeD == 0) {
            delete first;
            first = last = NULL;
            pastTheEnd.prev = NULL;
            return;
        }
        if (last->sizeB == 1) {
            if (last->prev) last->prev->next = last->next;
            if (last->next) last->next->prev = last->prev;
            delete last;
            last = pastTheEnd.prev;
        }
        else DeleteElement(last, last->sizeB - 1);
	}
	void push_front(const T &value) {
	    ++sizeD;
	    if (sizeD == 1) {
            ListBlock *newBlock = new ListBlock(value);
            first = last = newBlock;
            newBlock->next = &pastTheEnd;
            pastTheEnd.prev = newBlock;
            return;
	    }
        if (first->sizeB + 1 <= maxBlockSize) InsertElement(first, 0, value);
        else {
            ListBlock *newBlock = new ListBlock(value);
            newBlock->next = first;
            first->prev = newBlock;
            first = newBlock;
        }
	}
	void pop_front() {
        if (sizeD == 0) throw container_is_empty();
        --sizeD;
        if (sizeD == 0) {
            delete first;
            first = last = 0;
            pastTheEnd.prev = NULL;
            return;
        }
        if (first->sizeB == 1) {
            ListBlock *tmp = first;
            first = first->next;
            first->prev = NULL;
            delete tmp;
        }
        else DeleteElement(first, 0);
	}
};

}

#endif
