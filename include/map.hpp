#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template<class T>
inline T gmax(const T &a, const T &b) {
    return a > b ? a : b;
}
template<class T>
inline T gmin(const T &a, const T &b) {
    return a < b ? a : b;
}

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
public:
    typedef pair<const Key, T> value_type;
private:
    Compare CmpKey;
    bool CmpValue(const value_type &a, const value_type &b) const {
        return CmpKey(a.first, b.first);
    }
    bool EqualKey(const Key &a, const Key &b) const {
        return !(CmpKey(a, b) || CmpKey(b, a));
    }
    struct AvlTree {
        size_t sizeT;
        int h;
        value_type *v;
        AvlTree *l, *r, *prev, *next;
        AvlTree():
            v(NULL), sizeT(1), h(0), l(NULL), r(NULL), prev(NULL), next(NULL) {}
        AvlTree(const value_type &e):
            sizeT(1), h(0), l(NULL), r(NULL), prev(NULL), next(NULL) {
            v = new value_type(e);
        }
        AvlTree(const AvlTree &other):
            sizeT(other.sizeT), h(other.h), prev(NULL), next(NULL) {
            if (other.l) l = new AvlTree(*(other.l));
            else l = NULL;
            if (other.r) r = new AvlTree(*(other.r));
            else r = NULL;
            if (other.v) v = new value_type(*other.v);
            else v = NULL;
        }
        size_t GetSizeT(const AvlTree *p) {
            if (!p) return 0;
            return p -> sizeT;
        }
        int GetH(const AvlTree *p) {
            if (!p) return -1;
            return p -> h;
        }
        void Update() {
            sizeT = GetSizeT(l) + GetSizeT(r) + 1;
            h = gmax(GetH(l), GetH(r)) + 1;
        }
        ~AvlTree() {
            if (v) delete v;
            v = NULL;
            if (l) delete l;
            if (r) delete r;
        }
    };

    size_t GetSizeT(const AvlTree *p) {
        if (!p) return 0;
        return p -> sizeT;
    }
    int GetH(const AvlTree *p) {
        if (!p) return -1;
        return p -> h;
    }

    void LDR(AvlTree *now, AvlTree **ldrArr, int &ldrIndex) {
        if (now -> l) LDR(now -> l, ldrArr, ldrIndex);
        ldrArr[ldrIndex++] = now;
        if (now -> r) LDR(now -> r, ldrArr, ldrIndex);
    }
    void LL(AvlTree *&t) {
        AvlTree *t1 = t -> l;
        t -> l = t1 -> r;
        t1 -> r = t;
        t -> Update();
        t1 -> Update();
        t = t1;
    }
    void RR(AvlTree *&t) {
        AvlTree *t1 = t -> r;
        t -> r = t1 -> l;
        t1 -> l = t;
        t -> Update();
        t1 -> Update();
        t = t1;
    }
    void LR(AvlTree *&t) {
        RR(t -> l);
        LL(t);
    }
    void RL(AvlTree *&t) {
        LL(t -> r);
        RR(t);
    }
    AvlTree *Insert(AvlTree *&t, const value_type &v, AvlTree *fa, int lorr) {
        AvlTree *ret;
        if (t == NULL) {
            t = new AvlTree(v);
            if (lorr == 0) {
                t -> next = fa;
                t -> prev = fa -> prev;
                if (beginA == fa) beginA = t;
            }
            else {
                t -> prev = fa;
                t -> next = fa -> next;
            }
            if (t -> prev) t -> prev -> next = t;
            if (t -> next) t -> next -> prev = t;
            return t;
        }
        AvlTree *tmp;
        if (CmpValue(v, *(t -> v))) {
            ret = Insert(t -> l, v, t, 0);
            if (GetH(t -> l) - GetH(t -> r) == 2) {
                if (CmpValue(v, *(t -> l -> v))) LL(t);
                else LR(t);
            }
        }
        else if (CmpValue(*(t -> v), v)) {
            ret = Insert(t -> r, v, t, 1);
            if (GetH(t -> r) - GetH(t -> l) == 2) {
                if (CmpValue(*(t -> r -> v), v)) RR(t);
                else RL(t);
            }
        }
        t -> Update();
        return ret;
    }
    bool Remove(AvlTree *&t, const value_type &x, bool orig) {
        bool stop = false;
        int subTree;
        if (t == NULL) return true;
        if (CmpValue(x, *(t -> v))) {
            stop = Remove(t -> l, x, orig);
            subTree = 0;
        }
        else if (CmpValue(*(t -> v), x)) {
            stop = Remove(t -> r, x, orig);
            subTree = 1;
        }
        else if (t -> l && t -> r) {
            AvlTree *tmp = t -> r;
            while (tmp -> l) tmp = tmp -> l;
            stop = Remove(t -> r, *(tmp -> v), false);
            tmp -> l = t -> l;
            tmp -> r = t -> r;
            tmp -> Update();
            if (beginA == t) beginA = beginA -> next;
            if (t -> next) t -> next -> prev = t -> prev;
            if (t -> prev) t -> prev -> next = t -> next;
            t -> l = t -> r = NULL;
            delete t;
            t = tmp;
            subTree = 1;
        }
        else {
            AvlTree *oldNode = t;
            t = t -> l ? t -> l : t -> r;
            if (orig) {
                if (beginA == oldNode) beginA = beginA -> next;
                if (oldNode -> next) oldNode -> next -> prev = oldNode -> prev;
                if (oldNode -> prev) oldNode -> prev -> next = oldNode -> next;
                oldNode -> l = oldNode -> r = NULL;
                delete oldNode;
            }
            return false;
        }
        t -> Update();
        if (stop) return true;
        int bf;
        switch (subTree) {
        case 0:
            bf = GetH(t -> l) - GetH(t -> r) + 1;
            if (bf == 0) return true;
            if (bf == 1) return false;
            if (bf == -1) {
                int bfr = GetH(t -> r -> l) - GetH(t -> r -> r);
                switch (bfr) {
                case 0:
                    RR(t);
                    return true;
                    break;
                case -1:
                    RR(t);
                    return false;
                    break;
                default:
                    RL(t);
                    return false;
                }
            }
            break;
        case 1:
            bf = GetH(t -> l) - GetH(t -> r) - 1;
            if (bf == 0) return true;
            if (bf == -1) return false;
            if (bf == 1) {
                int bfl = GetH(t -> l -> l) - GetH(t -> l -> r);
                switch (bfl) {
                case 0:
                    LL(t);
                    return true;
                    break;
                case 1:
                    LL(t);
                    return false;
                    break;
                default:
                    LR(t);
                    return false;
                }
            }
            break;
        }
    }
    AvlTree* GetKth(AvlTree *t, int k) {
        if (k <= GetSizeT(t -> l)) return GetKth(t -> l, k);
        k -= GetSizeT(t -> l);
        if (k == 1) return t;
        --k;
        return GetKth(t -> r, k);
    }
    const AvlTree* GetKth(AvlTree *t, int k) const {
        if (k <= GetSizeT(t -> l)) return GetKth(t -> l, k);
        k -= GetSizeT(t -> l);
        if (k == 1) return t;
        --k;
        return GetKth(t -> r, k);
    }

    size_t sizeM;
    AvlTree pastTheEnd, *root, *beginA;

public:
	class const_iterator;
	class iterator {
	    friend class map;
	private:
	    map *ctn;
        AvlTree *p;
        iterator(map *ctnA, AvlTree *pA): ctn(ctnA), p(pA) {}
    public:
		iterator(): ctn(NULL), p(NULL) {}
		iterator(const iterator &other): ctn(other.ctn), p(other.p) {}
		iterator operator++(int) {
            iterator tmp(*this);
            if (p -> next == NULL) throw invalid_iterator();
            p = p -> next;
            return tmp;
		}
		iterator & operator++() {
            if (p -> next == NULL) throw invalid_iterator();
            p = p -> next;
            return *this;
		}
		iterator operator--(int) {
            iterator tmp(*this);
            if (p -> prev == NULL) throw invalid_iterator();
            p = p -> prev;
            return tmp;
		}
		iterator & operator--() {
		    if (p -> prev == NULL) throw invalid_iterator();
            p = p -> prev;
            return *this;
		}
		value_type & operator*() const {
            if (p -> v == NULL) throw invalid_iterator();
            return *(p -> v);
		}
		bool operator==(const iterator &rhs) const {
            return (ctn == rhs.ctn) && (p == rhs.p);
		}
		bool operator==(const const_iterator &rhs) const {
            return (ctn == rhs.ctn) && (p == rhs.p);
		}
		bool operator!=(const iterator &rhs) const {
            return (ctn != rhs.ctn) || (p != rhs.p);
		}
		bool operator!=(const const_iterator &rhs) const {
		    return (ctn != rhs.ctn) || (p != rhs.p);
		}
		value_type* operator->() const noexcept {
            return p -> v;
		}
	};
	class const_iterator {
        friend class map;
    private:
        const map *ctn;
        const AvlTree *p;
        const_iterator(const map *ctnA, const AvlTree *pA): ctn(ctnA), p(pA) {}
    public:
        const_iterator(): ctn(NULL), p(NULL) {}
        const_iterator(const const_iterator &other): ctn(other.ctn), p(other.p) {}
        const_iterator(const iterator &other): ctn(other.ctn), p(other.p) {}

        const_iterator operator++(int) {
            const_iterator tmp(*this);
            if (p -> next == NULL) throw invalid_iterator();
            p = p -> next;
            return tmp;
		}
		const_iterator & operator++() {
            if (p -> next == NULL) throw invalid_iterator();
            p = p -> next;
            return *this;
		}
		const_iterator operator--(int) {
            const_iterator tmp(*this);
            if (p -> prev == NULL) throw invalid_iterator();
            p = p -> prev;
            return tmp;
		}
		const_iterator & operator--() {
		    if (p -> prev == NULL) throw invalid_iterator();
            p = p -> prev;
            return *this;
		}
		const value_type & operator*() const {
            if (p -> v == NULL) throw invalid_iterator();
            return *(p -> v);
		}
		bool operator==(const iterator &rhs) const {
            return (ctn == rhs.ctn) && (p == rhs.p);
		}
		bool operator==(const const_iterator &rhs) const {
            return (ctn == rhs.ctn) && (p == rhs.p);
		}
		bool operator!=(const iterator &rhs) const {
            return (ctn != rhs.ctn) || (p != rhs.p);
		}
		bool operator!=(const const_iterator &rhs) const {
		    return (ctn != rhs.ctn) || (p != rhs.p);
		}
		const value_type* operator->() const noexcept {
            return p -> v;
		}
	};

	map(): root(NULL), beginA(&pastTheEnd), sizeM(0) {}
	map(const map &other): sizeM(other.sizeM) {
        if (other.root) {
            root = new AvlTree(*(other.root));
            AvlTree **nodes = new AvlTree*[sizeM];
            int ldrIndex = 0;
            LDR(root, nodes, ldrIndex);
            for (int i = 0; i < sizeM - 1; ++i) {
                nodes[i] -> next = nodes[i + 1];
                nodes[i + 1] -> prev = nodes[i];
            }
            nodes[0] -> prev = NULL;
            nodes[sizeM - 1] -> next = &pastTheEnd;
            beginA = nodes[0];
            pastTheEnd.next = NULL;
            pastTheEnd.prev = nodes[sizeM - 1];
            delete [] nodes;
        }
        else {
            root = NULL;
            pastTheEnd.prev = pastTheEnd.next = NULL;
            beginA = &pastTheEnd;
        }
	}

	map & operator=(const map &other) {
        if (&other == this) return *this;
        sizeM = other.sizeM;
        if (root) delete root;
        if (other.root) {
            root = new AvlTree(*(other.root));
            AvlTree **nodes = new AvlTree*[sizeM];
            int ldrIndex = 0;
            LDR(root, nodes, ldrIndex);
            for (int i = 0; i < sizeM - 1; ++i) {
                nodes[i] -> next = nodes[i + 1];
                nodes[i + 1] -> prev = nodes[i];
            }
            nodes[0] -> prev = NULL;
            nodes[sizeM - 1] -> next = &pastTheEnd;
            beginA = nodes[0];
            pastTheEnd.next = NULL;
            pastTheEnd.prev = nodes[sizeM - 1];
            delete [] nodes;
        }
        else {
            root = NULL;
            pastTheEnd.prev = pastTheEnd.next = NULL;
            beginA = &pastTheEnd;
        }
        return *this;
	}
	~map() {
        if (root) delete root;
    }

	T & at(const Key &key) {
        AvlTree *t = root;
        while (t && !EqualKey(t -> v -> first, key)) {
            if (CmpKey(key, t -> v -> first)) t = t -> l;
            else t = t -> r;
        }
        if (!t) throw index_out_of_bound();
        else return t -> v -> second;
	}
	const T & at(const Key &key) const {
        const AvlTree *t = root;
        while (t && !EqualKey(t -> v -> first, key)) {
            if (CmpKey(key, t -> v -> first)) t = t -> l;
            else t = t -> r;
        }
        if (!t) throw index_out_of_bound();
        else return t -> v -> second;
	}
	T & operator[](const Key &key) {
        AvlTree *t = root;
        while (t && !EqualKey(t -> v -> first, key)) {
            if (CmpKey(key, t -> v -> first)) t = t -> l;
            else t = t -> r;
        }
        if (t) return t -> v -> second;
        iterator newIt = insert(pair<Key, T>(key, T())).first;
        t = newIt.p;
        return t -> v -> second;
	}
	const T & operator[](const Key &key) const {
        const AvlTree *t = root;
        while (t && !EqualKey(t -> v -> first, key)) {
            if (CmpKey(key, t -> v -> first)) t = t -> l;
            else t = t -> r;
        }
        if (!t) throw index_out_of_bound();
        else return t -> v -> second;
	}

	iterator begin() {
        return iterator(this, beginA);
    }
	const_iterator cbegin() const {
        return const_iterator(this, beginA);
	}
	iterator end() {
        return iterator(this, &pastTheEnd);
	}
	const_iterator cend() const {
        return const_iterator(this, &pastTheEnd);
	}
	bool empty() const {
        return sizeM == 0;
	}
	size_t size() const {
        return sizeM;
	}
	void clear() {
        if (root) delete root;
        root = NULL;
        sizeM = 0;
        pastTheEnd.prev = pastTheEnd.next = NULL;
        beginA = &pastTheEnd;
	}
	pair<iterator, bool> insert(const value_type &value) {
        if (sizeM == 0) {
            ++sizeM;
            root = new AvlTree(value);
            beginA = root;
            root -> next = &pastTheEnd;
            pastTheEnd.prev = root;
            return pair<iterator, bool>(iterator(this, root), true);
        }
        iterator fd = find(value.first);
        if (fd.p != &pastTheEnd) return pair<iterator, bool>(fd, false);
        ++sizeM;
        AvlTree *newE = Insert(root, value, NULL, 0);
        return pair<iterator, bool>(iterator(this, newE), true);
	}
	void erase(iterator pos) {
        if (pos.ctn != this || pos.p == &pastTheEnd) throw index_out_of_bound();
        --sizeM;
        if (sizeM == 0) {
            delete root;
            root = NULL;
            beginA = &pastTheEnd;
            pastTheEnd.prev = NULL;
            return;
        }
        value_type tmp = *(pos.p -> v);
        Remove(root, tmp, true);
	}

	size_t count(const Key &key) const {
        if (find(key).p == &pastTheEnd) return 0;
        else return 1;
	}
	iterator find(const Key &key) {
        AvlTree *t = root;
        while (t && !EqualKey(t -> v -> first, key)) {
            if (CmpKey(key, t -> v -> first)) t = t -> l;
            else t = t -> r;
        }
        if (!t) return iterator(this, &pastTheEnd);
        else return iterator(this, t);
	}
	const_iterator find(const Key &key) const {
        const AvlTree *t = root;
        while (t && !EqualKey(t -> v -> first, key)) {
            if (CmpKey(key, t -> v -> first)) t = t -> l;
            else t = t -> r;
        }
        if (!t) return const_iterator(this, &pastTheEnd);
        else return const_iterator(this, t);
	}

    iterator getByRank(int k) {
        if (k <= 0 || k > sizeM) throw index_out_of_bound();
        return iterator(this, GetKth(root, k));
    }
    const_iterator getByRank(int k) const {
        if (k <= 0 || k > sizeM) throw index_out_of_bound();
        return const_iterator(this, GetKth(root, k));
    }
};

}

#endif
