#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {

template<class T>
void swap(T &a, T &b) {
    T tmp = a;
    a = b; b = tmp;
}

template<typename T, class Compare = std::less<T>>
class priority_queue {
private:
    struct Tree {
        T *x;
        int d;
        Tree *l, *r;

        Tree(): x(NULL), l(NULL), r(NULL) {}
        Tree(const T &e): x(new T(e)), d(0), l(NULL), r(NULL) {}
        ~Tree() {
            if (x) delete x;
            if (l) delete l;
            if (r) delete r;
        }
        void copy(const Tree &obj) {
            if (x) delete x;
            x = new T(*obj.x);
            d = obj.d;
            if (l) delete l;
            if (r) delete r;
            if (obj.l) {
                l = new Tree;
                l -> copy(*obj.l);
            }
            else l = NULL;
            if (obj.r) {
                r = new Tree;
                r -> copy(*obj.r);
            }
            else r = NULL;
        }
    };

    static int TreeDist(Tree *t) {
        if (t == NULL) return -1;
        return t -> d;
    }
    static Tree *TreeMerge(Tree *t1, Tree *t2) {
        Compare Cmp;
        if (t1 == NULL) return t2;
        if (t2 == NULL) return t1;
        if (Cmp(*(t1 -> x), *(t2 -> x))) swap(t1, t2);
        t1 -> r = TreeMerge(t1 -> r, t2);
        if (TreeDist(t1 -> l) < TreeDist(t1 -> r)) swap(t1 -> l, t1 -> r);
        t1 -> d = TreeDist(t1 -> r) + 1;
        return t1;
    }

    Tree *root;
    size_t n;
public:
	priority_queue(): root(NULL), n(0) {
	}
	priority_queue(const priority_queue &other): n(other.n) {
        if (other.root) {
            root = new Tree;
            root -> copy(*other.root);
        }
        else root = NULL;
	}
	~priority_queue() {
        if (root) delete root;
	}
	priority_queue &operator=(const priority_queue &other) {
        if (this == &other) return *this;
        n = other.n;
        if (root) delete root;
        if (other.root) {
            root = new Tree;
            root -> copy(*other.root);
        }
        else root = NULL;
        return *this;
	}
	const T & top() const {
        if (n == 0) throw container_is_empty();
        return *(root -> x);
	}
	void push(const T &e) {
	    ++n;
        Tree *p = new Tree(e);
        root = TreeMerge(root, p);
	}
	void pop() {
	    if (n == 0) throw container_is_empty();
        --n;
        Tree *tmp = root;
        root = TreeMerge(tmp -> l, tmp -> r);
        tmp -> l = tmp -> r = NULL;
        delete tmp;
	}
	size_t size() const {
        return n;
	}
	bool empty() const {
        return (n == 0);
	}
	void merge(priority_queue &other) {
        n += other.n;
        root = TreeMerge(root, other.root);
        other.root = NULL;
        other.n = 0;
	}
};

}

#endif
