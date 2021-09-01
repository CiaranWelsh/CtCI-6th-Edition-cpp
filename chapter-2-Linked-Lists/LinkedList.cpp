//
// Created by Ciaran on 30/08/2021.
//

#include <utility>
#include <gtest/gtest.h>

/**
 * @brief Implementation of a doubly linked list, based on the implementation in Data Structures textbook
 * @details
 *  - we go for a LinkedList class with a nested class implementation of a Node object.
 *  - We also use a nested class for the iterators and const_iterators, instead of simple pointers.
 *  - We maintain iterators and const iterators to head and tail nodes, which are one before and one after the first
 *    and last element respectively.
 *  - Implement rule of 5 as well as expected operations such as insert, delete.
 */

template<typename Object>
class LinkedList {
public:

    struct Node {
        Object data;
        Node *prev;
        Node *next;

        Node(const Object &d = Object{},
             Node *p = nullptr,
             Node *n = nullptr)
                : data{d}, prev{p}, next{n} {}

//        Node(Object &&d = Object{},
//                      Node *p = nullptr,
//                      Node *n = nullptr)
//                : data{std::move(d)}, prev{p}, next{n} {}

    };

    class const_iterator {
    public:

        const_iterator() : current(nullptr) {};

        /**
         * @brief defreference operator.
         * @details like the pointer implementation of
         * an iterator, we want to be able to get the value of
         * the item pointed to by the iterator using the
         * dereferencing operator.
         */
        const Object &operator*() {
            return retrieve();
        };

        const_iterator operator++() {
            // for it++ syntax
            // simply increment iterator to next item
            current = current->next;
            return *this;
        };

        const_iterator operator--() {
            // for it-- syntax
            // decrement the iterator
            current = current->prev;
            return *this;
        };

        const const_iterator operator++(int) {
            // for ++it syntax
            // return the iterator pointing to
            // current value *before* the move
            // to the new value.
            const_iterator old = *this;
            ++(*this);
            return old;
        }

        const const_iterator operator--(int) {
            // for ++it syntax
            // increment only occurs after
            // iterator has been retrieved.
            const_iterator old = *this;
            --(*this);
            return old;
        }

        bool operator==(const const_iterator &rhs) {
            return current == rhs.current;
        }

        bool operator!=(const const_iterator &rhs) {
            return !(*this == rhs);
        }


    protected:

        Node *current = nullptr;

        Object &retrieve() const {
            return current->data;
        };

        const_iterator(Node *p) : current(p) {};

        friend class LinkedList<Object>;
    };

    /**
     * @brief
     * @details an iterator IS-A const_iterator.
     * An iterator can be used instead of a const_iterator
     * whilst a const_iterator cannot be ised in place of
     * an iterator.
     */
    class iterator : public const_iterator {
    public:

        iterator() = default;

    protected:
        iterator(Node *p) : const_iterator(p) {};

        friend class LinkedList<Object>;

    };

    /**
     * Constructor and assignment operator section
     */

    LinkedList() {
        init();
    }

    ~LinkedList() {
        clear();
        delete head;
        delete tail;
    };

    LinkedList(const LinkedList &rhs) {
        init();
        for (auto &it: rhs) {
            push_back(it);
        }
    }

    LinkedList(LinkedList &&rhs) noexcept {
        init();
        for (auto &it: rhs) {
            push_back(std::move(it));
        }
    }

    LinkedList &operator=(const LinkedList &rhs) {
        // use the copy ctr
        LinkedList ll = rhs;
        // now we have rhs, this and ll.
        // We want to swap contents of this with our
        // new local ll
        std::swap(*this, ll);
        // so that ll will be properly destructed
        // and *this contains contents equal to rhs
        return *this;
    }

    LinkedList &operator=(LinkedList &&rhs) noexcept {
        // with move assignment, we just move the
        // state of the rhs.
        head = std::move(rhs.head);
        tail = std::move(rhs.tail);
        theSize = std::move(rhs.theSize);

    }

    iterator begin() {
        return {head->next};
    }

    const_iterator begin() const {
        return {head->next};
    }

    iterator end() {
        return {tail};
    }

    const_iterator end() const {
        return {tail};
    }

    int size() const {
        return theSize;
    }

    bool empty() const {
        return theSize == 0;
    }

    void clear() {
        while (!empty()) {
            pop_front();
        }
    }

    Object &front() {
        return *begin();
    }

    Object &front() const {
        return *begin();
    }

    Object &back() {
        return *--end();
    }

    Object &back() const {
        return *--end();
    }

    void push_front(const Object &obj) {
        insert(begin(), obj);
    }

    void push_front(Object &&obj) {
        insert(begin(), std::move(obj));
    }

    void push_back(const Object &obj) {
        insert(end(), obj);
    }

    void push_back(Object &&obj) {
        insert(end(), std::move(obj));
    }

    void pop_front() {
        erase(begin());
    }

    void pop_back() {
        erase(--end());
    }

    iterator insert(iterator itr, const Object &obj) {
        // creates a new node
        // ll = head, n1, n2, tail
        // ll = head, n1, [n], n2, tail
        // if itr points to n2, we want to add a new node n between n1 and n2.
        // this means that the following must happen
        //  n1->next = n
        //  n->prev = n1
        //  n->next = n2
        //  n2->prev = n
        //
        // rewrite the above in terms of current
        //  n1 is the same as current->prev.
        //  n2 is the same as current
        //
        //   current->prev->next = n
        //   n->prev = current->prev        in ctr
        //   n->next = current              in ctr
        //   current->prev = n
        Node *current = itr.current;
        Node *n = new Node(obj, current->prev, current);
        theSize++;
        current->prev->next = current->prev = n;
        return {n};
    }

    iterator insert(iterator itr, Object &&obj) {
        Node *current = itr.current;
        Node *n = new Node(std::move(obj), current->prev, current);
        theSize++;
        current->prev = current->prev->next = n;
        return {n};
    }

    iterator erase(iterator itr) {
        /**
         * ll = head, n1 , n2, tail
         * If itr points to n1,
         *  head->next needs to point to n2
         *  n2-> prev needs to point to head.
         *
         * head is the same as current->prev
         * n2 is the same as current->next
         *
         */
        Node *current = itr.current;
        iterator retVal = (current->next);
        current->prev->next = current->next;
        current->next->prev = current->prev;
        delete current;
        theSize--;
        return retVal;
    }

    iterator erase(iterator from, iterator to) {
        for (iterator itr = from; itr != to;) {
            erase(itr);
        }
    }


private:

    void init() {
        theSize = 0;
        head = new Node;
        tail = new Node;
        head->next = tail;
        tail->prev = head;

    }

    int theSize = 0;
    Node *head = nullptr;
    Node *tail = nullptr;

};

class LinkedListTests : public ::testing::Test {
public:
    LinkedListTests() = default;
};

TEST_F(LinkedListTests, Instantiate) {
    LinkedList<int> ll;
    ASSERT_EQ(ll.size(), 0);
}

TEST_F(LinkedListTests, insert) {
    LinkedList<int> ll;
    auto it = ll.begin();
    ll.insert(it, 5);
    ASSERT_EQ(*ll.begin(), 5);
}

TEST_F(LinkedListTests, erase) {
    LinkedList<int> ll;
    ll.insert(ll.begin(), 5);
    ASSERT_FALSE(ll.empty());
    ll.erase(ll.begin());
    ASSERT_TRUE(ll.empty());
}

TEST_F(LinkedListTests, push_front) {
    LinkedList<int> ll;
    ll.push_front(5);
    ASSERT_EQ(*ll.begin(), 5);
}

TEST_F(LinkedListTests, push_back) {
    LinkedList<int> ll;
    ll.push_back(5);
    ASSERT_EQ((*--ll.end()), 5);
}


TEST_F(LinkedListTests, CopyCtr) {
    // ll = [head, n1, tail]
    LinkedList<int> ll1;
    ll1.push_back(4);
    LinkedList<int> ll2 = ll1;
    ASSERT_EQ(1, ll2.size());
    // check from the front
    ASSERT_EQ(4, *ll2.begin());
    // check from the back
    ASSERT_EQ(4, *--ll2.end());

}

TEST_F(LinkedListTests, CopyAssign) {
    // ll = [head, n1, tail]
    LinkedList<int> ll1;
    ll1.push_back(4);

    LinkedList<int> ll2;
    ll2 = ll1;

    ASSERT_EQ(1, ll2.size());
    // check from the front
    ASSERT_EQ(4, *ll2.begin());
    // check from the back
    ASSERT_EQ(4, *--ll2.end());

}

TEST_F(LinkedListTests, MoveCtr) {

}

TEST_F(LinkedListTests, MoveAssign) {

}

TEST_F(LinkedListTests, ConstIteratorDereference) {
    LinkedList<int> ll;
    ll.push_back(4);
    ASSERT_EQ(1, ll.size());
    LinkedList<int>::const_iterator it = ll.begin();
    ASSERT_EQ(4, *it);
}

TEST_F(LinkedListTests, ConstIteratorIncrementOpLHS) {
    LinkedList<int> ll;
    ll.push_back(4);
    ll.push_back(5);
    ASSERT_EQ(2, ll.size());
    LinkedList<int>::const_iterator it = ++ll.begin();
    ASSERT_EQ(5, *it);
}
TEST_F(LinkedListTests, ConstIteratorIncrementOpRHS) {
    LinkedList<int> ll;
    ll.push_back(4);
    ll.push_back(5);
    ASSERT_EQ(2, ll.size());
    // ++itr returns the iterator pointing to the current
    // value and then increments the iterator.
    //
    // So it here will point to 4
    LinkedList<int>::const_iterator it = ll.begin();
    ASSERT_EQ(4, *it);
    std::cout << (*it) << std::endl;
    std::cout << *it << std::endl;
}


TEST_F(LinkedListTests, IteratorFromBegin) {

}

TEST_F(LinkedListTests, IteratorFromEnd) {}

TEST_F(LinkedListTests, ConstIteratorFromBegin) {}

TEST_F(LinkedListTests, ConstIteratorFromEnd) {}

TEST_F(LinkedListTests, IteratorIncrementOp1Behind) {

}
TEST_F(LinkedListTests, IteratorIncrementOp2InFront) {}





