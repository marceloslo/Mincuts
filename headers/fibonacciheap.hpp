/**
 * Fibonacci Heap
 * Copyright (c) 2014, Emmanuel Benazera beniz@droidnik.fr, All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 */

/**
*
* Esse heap foi modificado por nós para ser um max heap no lugar de um min heap
*
**/

#ifndef FIBOHEAP_H
#define FIBOHEAP_H

#include <cstddef>
#include <math.h>
#include <limits>
#include <iostream>

template<class T, class Comp = std::greater<T>>
class FibHeap
{
public:

    // node
    class FibNode
    {
    public:
        FibNode(T k, void* pl)
            :key(std::move(k)), mark(false), p(nullptr), left(nullptr), right(nullptr), child(nullptr), degree(-1), payload(pl)
        {
        }

        ~FibNode()
        {
        }

        T key;
        bool mark;
        FibNode* p;
        FibNode* left;
        FibNode* right;
        FibNode* child;
        int degree;
        void* payload;
    }; // end FibNode

    FibHeap() : FibHeap(std::greater<T>())
    {
    }

    FibHeap(Comp comp)
        :n(0), max(nullptr), comp(comp)
    {
    }

    ~FibHeap()
    {
        clear();
    }

    void clear() {
        // delete all nodes.
        delete_fibnodes(max);
        max = nullptr;
        n = 0;
    }

    void delete_fibnodes(FibNode* x)
    {
        if (!x)
            return;

        FibNode* cur = x;
        while (true)
        {
            /*std::cerr << "cur: " << cur << std::endl;
              std::cerr << "x: " << x << std::endl;*/
            if (cur->left && cur->left != x)
            {
                //std::cerr << "cur left: " << cur->left << std::endl;
                FibNode* tmp = cur;
                cur = cur->left;
                if (tmp->child)
                    delete_fibnodes(tmp->child);
                delete tmp;
            }
            else
            {
                if (cur->child)
                    delete_fibnodes(cur->child);
                delete cur;
                break;
            }
        }
    }

    /*
     * insert(x)
     * 1. x.degree = 0
     * 2. x.p = NIL
     * 3. x.child = NIL
     * 4. x.mark = FALSE
     * 5. if H.max == NIL
     * 6. 	create a root list for H containing just x
     * 7. 	H.max = x
     * 8. else insert x into H's root list
     * 9. 	if x.key < H.max.key
     *10. 		H.max = x
     *11. H.n = H.n + 1
     */
    void insert(FibNode* x)
    {
        // 1
        x->degree = 0;
        // 2
        x->p = nullptr;
        // 3
        x->child = nullptr;
        // 4
        x->mark = false;
        // 5
        if (max == nullptr)
        {
            // 6, 7
            max = x->left = x->right = x;
        }
        else
        {
            // 8
            max->left->right = x;
            x->left = max->left;
            max->left = x;
            x->right = max;
            // 9
            if (comp(x->key, max->key))
            {
                // 10
                max = x;
            }
        }
        // 11
        ++n;
    }

    /*
     * The maximum node of the heap.
     */
    FibNode* maximum()
    {
        return max;
    }

    /*
     * union_fibheap(H1,H2)
     * 1. H = MAKE-FIB-HEAP()
     * 2. H.max = H1.max
     * 3. concatenate the root list of H2 with the root list of H
     * 4. if (H1.max == NIL) or (H2.max != NIL and H2.max.key < H1.max.key)
     * 5. 	H.max = H2.max
     * 6. H.n = H1.n + H2.n
     * 7. return H
     */
    static FibHeap* union_fibheap(FibHeap* H1, FibHeap* H2)
    {
        // 1
        FibHeap* H = new FibHeap();
        // 2
        H->max = H1->max;
        // 3
        if (H->max != nullptr && H2->max != nullptr)
        {
            H->max->right->left = H2->max->left;
            H2->max->left->right = H->max->right;
            H->max->right = H2->max;
            H2->max->left = H->max;
        }
        // 4
        if (H1->max == nullptr || (H2->max != nullptr && H1->comp(H2->max->key, H1->max->key)))
        {
            // 5
            H->max = H2->max;
        }
        // 6
        H->n = H1->n + H2->n;
        // 7
        return H;
    }

    /*
     * extract_max
     * 1. z = H.max
     * 2. if z != NIL
     * 3. 	for each child x of z
     * 4. 		add x to the root list of H
     * 5. 		x.p = NIL
     * 6. 	remove z from the root list of H
     * 7.		if z == z.right
     * 8. 		H.max = NIL
     * 9. 	else H.max = z.right
     *10. 		CONSOLIDATE(H)
     *11. 	H.n = H.n - 1
     *12. return z
     */
    FibNode* extract_max()
    {
        FibNode* z, * x, * next;
        FibNode** childList;

        // 1
        z = max;
        // 2
        if (z != nullptr)
        {
            // 3
            x = z->child;
            if (x != nullptr)
            {
                childList = new FibNode * [z->degree];
                next = x;
                for (int i = 0; i < (int)z->degree; i++)
                {
                    childList[i] = next;
                    next = next->right;
                }
                for (int i = 0; i < (int)z->degree; i++)
                {
                    x = childList[i];
                    // 4
                    max->left->right = x;
                    x->left = max->left;
                    max->left = x;
                    x->right = max;
                    // 5
                    x->p = nullptr;
                }
                delete[] childList;
            }
            // 6
            z->left->right = z->right;
            z->right->left = z->left;
            // 7
            if (z == z->right)
            {
                // 8
                max = nullptr;
            }
            else
            {
                // 9
                max = z->right;
                // 10
                consolidate();
            }
            // 11
            n--;
        }
        // 12
        return z;
    }

    /*
     * consolidate
     * 1. let A[0 . . D(H.n)] be a new array
     * 2. for i = 0 to D(H.n)
     * 3. 	A[i] = NIL
     * 4. for each node w in the root list of H
     * 5. 	x = w
     * 6. 	d = x.degree
     * 7. 	while A[d] != NIL
     * 8. 		y = A[d]
     * 9. 		if x.key > y.key
     *10.			exchange x with y
     *11. 		FIB-HEAP-LINK(H,y,x)
     *12. 		A[d] = NIL
     *13. 		d = d + 1
     *14. 	A[d] = x
     *15. H.max = NIL
     *16. for i = 0 to D(H.n)
     *17. 	if A[i] != NIL
     *18. 		if H.max == NIL
     *19. 			create a root list for H containing just A[i]
     *20. 			H.max = A[i]
     *21. 		else insert A[i] into H's root list
     *22. 			if A[i].key < H.max.key
     *23. 				H.max = A[i]
     */
    void consolidate()
    {
        FibNode* w, * next, * x, * y, * temp;
        FibNode** A, ** rootList;
        // Max degree <= log base golden ratio of n
        int d, rootSize;
        int max_degree = static_cast<int>(floor(log(static_cast<double>(n)) / log(static_cast<double>(1 + sqrt(static_cast<double>(5))) / 2)));

        // 1
        A = new FibNode * [max_degree + 2]; // plus two both for indexing to max degree and so A[max_degree+1] == NIL
        // 2, 3
        std::fill_n(A, max_degree + 2, nullptr);
        // 4
        w = max;
        rootSize = 0;
        next = w;
        do
        {
            rootSize++;
            next = next->right;
        } while (next != w);
        rootList = new FibNode * [rootSize];
        for (int i = 0; i < rootSize; i++)
        {
            rootList[i] = next;
            next = next->right;
        }
        for (int i = 0; i < rootSize; i++)
        {
            w = rootList[i];
            // 5
            x = w;
            // 6
            d = x->degree;
            // 7
            while (A[d] != nullptr)
            {
                // 8
                y = A[d];
                // 9
                if (comp(y->key, x->key))
                {
                    // 10
                    temp = x;
                    x = y;
                    y = temp;
                }
                // 11
                fib_heap_link(y, x);
                // 12
                A[d] = nullptr;
                // 13
                d++;
            }
            // 14
            A[d] = x;
        }
        delete[] rootList;
        // 15
        max = nullptr;
        // 16
        for (int i = 0; i < max_degree + 2; i++)
        {
            // 17
            if (A[i] != nullptr)
            {
                // 18
                if (max == nullptr)
                {
                    // 19, 20
                    max = A[i]->left = A[i]->right = A[i];
                }
                else
                {
                    // 21
                    max->left->right = A[i];
                    A[i]->left = max->left;
                    max->left = A[i];
                    A[i]->right = max;
                    // 22
                    if (comp(A[i]->key, max->key))
                    {
                        // 23
                        max = A[i];
                    }
                }
            }
        }
        delete[] A;
    }

    /*
     * fib_heap_link(y,x)
     * 1. remove y from the root list of heap
     * 2. make y a child of x, incrementing x.degree
     * 3. y.mark = FALSE
     */
    void fib_heap_link(FibNode* y, FibNode* x)
    {
        // 1
        y->left->right = y->right;
        y->right->left = y->left;
        // 2
        if (x->child != nullptr)
        {
            x->child->left->right = y;
            y->left = x->child->left;
            x->child->left = y;
            y->right = x->child;
        }
        else
        {
            x->child = y;
            y->right = y;
            y->left = y;
        }
        y->p = x;
        x->degree++;
        // 3
        y->mark = false;
    }


    /*
     * decrease_key(x,k)
     * 1. if k > x.key
     * 2. 	error "new key is greater than current key"
     * 3. x.key = k
     * 4. y = x.p
     * 5. if y != NIL and x.key < y.key
     * 6. 	CUT(H,x,y)
     * 7. 	CASCADING-CUT(H,y)
     * 8. if x.key < H.max.key
     * 9. 	H.max = x
     */
    void increase_key(FibNode* x, T k)
    {
        FibNode* y;

        // 1
        if (comp(x->key, k))
        {
            // 2
            // error( "new key is greater than current key" );
            return;
        }
        // 3
        x->key = std::move(k);
        // 4
        y = x->p;
        // 5
        if (y != nullptr && comp(x->key, y->key))
        {
            // 6
            cut(x, y);
            // 7
            cascading_cut(y);
        }
        // 8
        if (comp(x->key, max->key))
        {
            // 9
            max = x;
        }
    }

    /*
     * cut(x,y)
     * 1. remove x from the child list of y, decrementing y.degree
     * 2. add x to the root list of H
     * 3. x.p = NIL
     * 4. x.mark = FALSE
     */
    void cut(FibNode* x, FibNode* y)
    {
        // 1
        if (x->right == x)
        {
            y->child = nullptr;
        }
        else
        {
            x->right->left = x->left;
            x->left->right = x->right;
            if (y->child == x)
            {
                y->child = x->right;
            }
        }
        y->degree--;
        // 2
        max->right->left = x;
        x->right = max->right;
        max->right = x;
        x->left = max;
        // 3
        x->p = nullptr;
        // 4
        x->mark = false;
    }

    /*
      * cascading_cut(y)
      * 1. z = y.p
      * 2. if z != NIL
      * 3. 	if y.mark == FALSE
      * 4. 		y.mark = TRUE
      * 5. 	else CUT(H,y,z)
      * 6. 		CASCADING-CUT(H,z)
      */
    void cascading_cut(FibNode* y)
    {
        FibNode* z;

        // 1
        z = y->p;
        // 2
        if (z != nullptr)
        {
            // 3
            if (y->mark == false)
            {
                // 4
                y->mark = true;
            }
            else
            {
                // 5
                cut(y, z);
                // 6
                cascading_cut(z);
            }
        }
    }

    /*
     * set to infinity so that it hits the top of the heap, then easily remove.
     */
    void remove_fibnode(FibNode* x)
    {
        decrease_key(x, std::numeric_limits<T>::max());
        FibNode* fn = extract_max();
        delete fn;
    }

    /*
     * mapping operations to STL-compatible signatures.
     */
    bool empty() const
    {
        return n == 0;
    }

    FibNode* topNode()
    {
        return maximum();
    }

    T& top()
    {
        return maximum()->key;
    }

    void pop()
    {
        if (empty())
            return;
        FibNode* x = extract_max();
        if (x)
            delete x;
    }

    FibNode* push(T k, void* pl)
    {
        FibNode* x = new FibNode(std::move(k), pl);
        insert(x);
        return x;
    }

    FibNode* push(T k)
    {
        return push(std::move(k), nullptr);
    }

    unsigned int size()
    {
        return (unsigned int)n;
    }

    int n;
    FibNode* max;
    Comp comp;

};

#endif