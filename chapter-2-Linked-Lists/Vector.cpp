//
// Created by Ciaran on 29/08/2021.
//
#include "gtest/gtest.h"

template<typename Object>
class Vector {
public:

    /**
     * @brief Construct a Vector<Object> with
     * @param initSize elements.
     * @details The Vector's capacity is set to @param initSize +
     * SPARE_CAPACITY, which is set to 16 by default.
     */
    explicit Vector(int initSize = 0)
            : theSize{initSize},
              theCapacity{initSize + SPARE_CAPACITY} {
        objects = new Object[theCapacity];
    };

    /**
     * @brief destructor needs to delete the heap
     * allocated array
     */
    ~Vector() {
        delete[] objects;
    }

    /**
     * @brief copy constructor
     * @details O(n) complexity.
     */
    Vector(const Vector &rhs)
            : theSize{rhs.theSize},
              theCapacity{rhs.theCapacity},
              objects{nullptr} {
        objects = new Object[theCapacity];
        for (int i = 0; i < size(); i++) {
            objects[i] = rhs.objects[i];
        }
    }

    /**
     * @details Copy assignment is defined in terms
     * of the copy constructor using the copy and swap idiom.
     */
    Vector &operator=(const Vector &rhs) {
        // no equality operator support so we do not
        // check for self assignment.
        Vector copy = rhs; // make a copy
        std::swap(*this, copy);
        return *this;
    }

    /**
     * @brief Move constructor.
     */
    Vector(Vector &&rhs) noexcept
            : theSize(rhs.theSize),
              theCapacity(rhs.theCapacity),
              objects(rhs.objects) {
        // ensure size, capacity and objects are zero'd / nulled
        rhs.theSize = 0;
        rhs.theCapacity = 0;
        rhs.objects = nullptr;
    }

    /**
     * @brief Move assignment operator. Uses swap idiom for strong exception safety
     */
    Vector &operator=(Vector &&rhs) noexcept {
        std::swap(theSize, rhs.theSize);
        std::swap(theCapacity, rhs.theCapacity);
        std::swap(objects, rhs.objects);
        return *this;
    }

    void reserve(int newCapacity) {
        if (newCapacity <= size()) {
            return;
        }
        // create the new array
        auto *newArr = new Object[newCapacity];
        // populate new Array
        for (int i = 0; i < size(); i++) {
            newArr[i] = std::move(objects[i]);
        }
        theCapacity = newCapacity;
        std::swap(objects, newArr);
        delete[] newArr;
    }

    /**
     * @brief change the size of this vector to @param newSize.
     * @details If the @param newSize is greater than the capacity of the current
     * Vector then we need to increase the capacity of the vector.
     * When the @param newSize is less than the current, then we do not need
     * to remove anything. We just assign the @param newSize to size and any
     * iterations over the vector will only go up to newSize. Memory will be
     * appropriately cleared up with the destructor.
     */
    void resize(int newSize) {
        if (newSize > capacity()) {
            int newCapacity = capacity();
            while (newCapacity < newSize) {
                newCapacity *= 2;
            }
            reserve(newCapacity);
        }
        theSize = newSize;
    }

    Object &operator[](int index) {
        return objects[index];
    }

    const Object &operator[](int index) const {
        return objects[index];
    }

    bool empty() {
        return size() == 0;
    }

    int size() const {
        return theSize;
    }

    int capacity() const {
        return theCapacity;
    }

    void push_back(const Object &obj) {
        if (theSize == theCapacity) {
            // we always want the capacity to be at least one more than size. This means we can
            // always have our end iterator pointing to theSize'th element (one more than the index of the
            // last element
            reserve(2 * theCapacity + 1);
        }
        objects[theSize++] = obj;
    }

    void push_back(Object &&obj) {
        if (theSize == theCapacity) {
            // when current size
            reserve(2 * theCapacity + 1);
        }
        objects[theSize++] = std::move(obj);
    }

    /**
     * @brief remove the last element from the vector
     * @details the removal doesn't actually reclaim space
     * or reduce the size of the Objects* array. Instead
     * the size is just decremented by 1 meaning any iterations
     * will not see elements greater than one less than the Size.
     */
    void pop_back() {
        theSize--;
    }

    Object &back() {
        return objects[theSize - 1];
    }

    /**
     * @brief we use a simple alias of Object* for
     * iterator and const iterator. All iterators are
     * const_iterators. That is:
     *      const_iterator <|-- iterator.
     *      an iterator IS-A const_iterator
     */
    using iterator = Object *;
    using const_iterator = const Object *;

    /**
     * @brief pointer to the first element in the objects array
     */
    iterator begin() {
        return &objects[0];
    }

    /**
     * @brief const pointer to the first element in the objects array
     */
    const_iterator begin() const {
        return &objects[0];
    }

    /**
     * @brief pointer to one past the last element in the objects array
     */
    iterator end() {
        return &objects[theSize];
    }

    /**
     * @brief const pointer to one past the last element in the objects array
     */
    iterator end() const {
        return &objects[theSize];
    }

    int SPARE_CAPACITY = 16;
private:
    int theSize;
    int theCapacity;
    Object *objects;
};


class VectorTests : public ::testing::Test {
public:
    VectorTests() = default;

};

TEST_F(VectorTests, ConstructorInitSize) {
    Vector<int> v(4);
    ASSERT_EQ(4, v.size());
}

TEST_F(VectorTests, ConstructorInitCapacity) {
    Vector<int> v(4);
    ASSERT_EQ(20, v.capacity());
}

TEST_F(VectorTests, CheckCopyCtr) {
    Vector<int> v(8);
    Vector<int> copy(v);
    /**
     * Note I don't think we can implement equality
     * operators for vector class. How do we know what
     * equality means for arbitrary type Object?
     *
     * Instead this test should just check for equality
     * between the arrays stored by the Vector.
     */
    ASSERT_EQ(8, copy.size());
    ASSERT_EQ(24, copy.capacity());
}


/**
 * @brief a quick note on the copy and swap idiom.
 * V1 is being copied int v2 via assignment. This means
 * that v2's original state must be dealt with. This
 * can be handled elegantly by first using the copy
 * constructor to make a local copy of the data in v1
 * and then swapping that with the data in v2. V2 destructs
 * the old state and subsequently v1 and v2 are the same.
 */
TEST_F(VectorTests, CopyAsignment) {
    Vector<int> v1(4);
    Vector<int> v2(5);
    v2 = v1;
    ASSERT_EQ(v2.size(), 4);
}


TEST_F(VectorTests, MoveCtr) {
    Vector<int> v1(3);
    Vector<int> v2 = std::move(v1);
    ASSERT_EQ(v2.size(), 3);
}

/**
 * @brief A note on the move assignment swap idiom.
 * V1 here is move assigned into v2. To implement this,
 * we actually swap the contents of v1 and v2. V2 persists
 * with 3 empty slots available for storing int's, and v2 persists
 * with 5. We now let the destructor for v1 take care of
 * reclaiming resources associated with the old v2.
 */
TEST_F(VectorTests, MoveAssignmentOp) {
    Vector<int> v1(3);
    Vector<int> v2(5);
    v2 = std::move(v1);
    ASSERT_EQ(v2.size(), 3);
}

/**
 * @brief check that when new capacity is requested
 * which is less than the original capacity,
 * we do not reduce the size. This is inefficient,
 * we might as well just keep the original capacity.
 */
TEST_F(VectorTests, ReserveNewCapacityLessThanOriginal) {
    // changes the capacity of the Vector.
    Vector<int> v(5);
    v.reserve(3);
    ASSERT_EQ(v.capacity(), 5 + v.SPARE_CAPACITY);
}

/**
 * @brief similarly to the case when new capacity is less than
 * the original, when new capcity is the same as the original
 * then we might as well do nothing and keep the original.
 */
TEST_F(VectorTests, ReserveNewCapacityEqualToOriginal) {
    // changes the capacity of the Vector.
    Vector<int> v(5);
    v.reserve(5);
    ASSERT_EQ(v.capacity(), 5 + v.SPARE_CAPACITY);
}

/**
 * @brief Most cases, a user requests a new capacity that is greater
 * than the original. In this case, we need to allocate a new array
 * of appropriate size and copy the contents of the old array into the new.
 * The old array is deallocated and the new persists, with its extra capcity.
 */
TEST_F(VectorTests, ReserveNewCapacityGreaterThanOriginal) {
    // changes the capacity of the Vector.
    Vector<int> v(5);
    v.reserve(8);
    ASSERT_EQ(v.capacity(), 8);
}

/**
 * @brief capacity stays the same, even if the size is different
 */
TEST_F(VectorTests, ResizeNewSizeLessThanOldCapacityCheckCapacityHasNotchanged) {
    Vector<int> v(10);
    v.resize(3);
    ASSERT_EQ(v.capacity(), 10 + v.SPARE_CAPACITY);
}

/**
 * Size has been reduced
 */
TEST_F(VectorTests, ResizeNewSizeLessThanOldCapacityCheckSize) {
    Vector<int> v(10);
    v.resize(3);
    ASSERT_EQ(v.size(), 3);
}

/**
 */
TEST_F(VectorTests, ResizeNewSizeGreaterThanOldCapacity) {
    // original size is 3 + spare capacity of 16
    Vector<int> v(3);
    ASSERT_EQ(3 + v.SPARE_CAPACITY, v.capacity());
    v.resize(100);
    // we double the new capacity until its greater than 100. That is
    // 19*2= 38
    // 38*2 = 76
    // 76*2 = 152
    ASSERT_EQ(2 * 2 * 2 * (3 + v.SPARE_CAPACITY), v.capacity());
}

TEST_F(VectorTests, PushBackWhenSize0) {
    Vector<int> v;
    v.push_back(8);
    ASSERT_EQ(v[0], 8);
}


TEST_F(VectorTests, PushBackWhenSizeEqualCapacityCheckValue) {
    Vector<int> v(16);
    // 0 to 15
    for (int i = 0; i < v.SPARE_CAPACITY - 1; i++) { // 0 + spare capacity normalised for 0 indexed C++
        v[i] = i;
    }

    v.push_back(v.SPARE_CAPACITY); // contains 16
    ASSERT_EQ(v[v.SPARE_CAPACITY], v.SPARE_CAPACITY);
}

TEST_F(VectorTests, PushBackWhenSizeEqualCapacityCheckSize) {
    Vector<int> v(16);
    // 0 to 15
    for (int i = 0; i < v.SPARE_CAPACITY - 1; i++) { // 0 + spare capacity normalised for 0 indexed C++
        v[i] = i;
    }

    v.push_back(v.SPARE_CAPACITY); // contains 16
    ASSERT_EQ(v.size(), v.SPARE_CAPACITY + 1); // size should be 17 now - one more than the spare capacity
}

TEST_F(VectorTests, PushBackWhenSizeEqualCapacityCheckCapacity) {
    // create empty vector. Its size is 0, but capacity equals v.SPARE_CAPACITUY == 16.
    Vector<int> v;
    // we now resize to the same size as SPARE_CAPACITY so that size == capacity
    v.resize(v.SPARE_CAPACITY);

    // fill our vector with numbers - 0 to 15
    for (int i = 0; i < v.SPARE_CAPACITY - 1; i++) { // 0 + spare capacity normalised for 0 indexed C++
        v[i] = i;
    }

    // double check that size == capacity
    ASSERT_EQ(v.size(), v.capacity());

    // now adding another element to v will increase its capacity.
    v.push_back(v.SPARE_CAPACITY); // contains 16
    ASSERT_EQ(v.capacity(), 2 * v.SPARE_CAPACITY + 1);
}

TEST_F(VectorTests, PushBackWithMoveSemanticsWhenSize0) {
    Vector<std::string> v;
    std::string x = "New String";
    v.push_back(std::move(x));
    ASSERT_STREQ(v[0].c_str(), "New String");
}


TEST_F(VectorTests, PushBackWithMoveSemanticsWhenSizeEqualCapacityCheckValue) {
    Vector<std::string> v(16);
    // 0 to 15
    for (int i = 0; i < v.SPARE_CAPACITY - 1; i++) { // 0 + spare capacity normalised for 0 indexed C++
        std::ostringstream os;
        os << i;
        v[i] = os.str();
    }
    std::string x = "16";
    v.push_back(std::move(x)); // contains 16
    ASSERT_STREQ((v[v.SPARE_CAPACITY]).c_str(), "16");
}

TEST_F(VectorTests, PushBackWithMoveSemanticsWhenSizeEqualCapacityCheckSize) {
    Vector<std::string> v(16);
    // 0 to 15
    for (int i = 0; i < v.SPARE_CAPACITY - 1; i++) { // 0 + spare capacity normalised for 0 indexed C++
        std::ostringstream os;
        os << i;
        v[i] = os.str();
    }

    std::string x = "16";
    v.push_back(std::move(x)); // contains 16
    ASSERT_EQ(v.size(), v.SPARE_CAPACITY + 1); // size should be 17 now - one more than the spare capacity
}

TEST_F(VectorTests, PushBackWithMoveSemanticsWhenSizeEqualCapacityCheckCapacity) {
    Vector<std::string> v(16);
    // 0 to 15
    for (int i = 0; i < v.SPARE_CAPACITY - 1; i++) { // 0 + spare capacity normalised for 0 indexed C++
        std::ostringstream os;
        os << i;
        v[i] = os.str();
    }

    std::string x = "16";
    v.push_back(std::move(x)); // contains 16
    ASSERT_EQ(v.capacity(), 2 * v.SPARE_CAPACITY + 1);
}


TEST_F(VectorTests, CheckEmptyWhenTrue) {
    Vector<std::string> v;
    ASSERT_TRUE(v.empty());
}

TEST_F(VectorTests, CheckEmptyWhenFalse) {
    Vector<std::string> v;
    v.push_back("3");
    ASSERT_FALSE(v.empty());
}

TEST_F(VectorTests, CheckBeginIterator){
    Vector<int> v(10);
    // we first need to populate the vector before we can use the iterator
    for (int i = 0; i< 10; i++){
        v[i] = i;
    }
    int counter = 0;
    for (Vector<int>::iterator it = v.begin(); it != v.end(); it++){
        // does the equality operation and then increments
        // (opposed to ++counter which increments first)
        ASSERT_EQ(*it, counter++);
    }
}






















