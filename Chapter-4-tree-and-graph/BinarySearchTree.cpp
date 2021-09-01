//
// Created by Ciaran on 31/08/2021.
//

#include "gtest/gtest.h"
#include <random>
#include <chrono>
/**
 * note we (like the STL) use L value reference
 * and R value reference version of a method
 */


/**
 * @brief a BinarySearchTree in C++
 *
 * @details We use the technique of having private member functions pass a pointer
 * variable using call-by-reference. Then, public member functions can pass a pointer to
 * the root to the private recursive member functions. In other words, the private member
 * functions are called by the public member and are recursive.
 *
 *
 */
template<typename Object, typename Comparator=std::less<Object>>
class BinarySearchTree {
public:

    BinarySearchTree()
            : root(nullptr) {};

    BinarySearchTree(const BinarySearchTree &rhs)
            : root(nullptr) {
        root = clone(rhs.root);
    }

    BinarySearchTree &operator=(const BinarySearchTree &rhs) {
        BinarySearchTree copy = rhs;
        std::swap(*this, rhs);
        return *this;
    }

//    BinarySearchTree(BinarySearchTree &&rhs) noexcept
//            : root(nullptr) {
//        clone();
//    }
//
//    BinarySearchTree &operator=(BinarySearchTree &&rhs) noexcept;

    ~BinarySearchTree() {
        makeEmpty();
    };

    const Object &findMin() const {
        return findMin(root)->element;
    }

    const Object &findMax() const {
        return findMax(root)->element;
    }

    bool contains(const Object &x) {
        return contains(x, root);
    }

    bool empty() {
        return root == nullptr;
    }

    void printTree() {
        printTree("", root, false);
    }

    void makeEmpty() {
        makeEmpty(root);
    }

    void insert(const Object &x) {
        insert(x, root);
    }

    void insert(Object &&x) {
        insert(std::move(x), root);
    }

    void remove(const Object &x) {
        remove(x, root);
    }

private:

    struct BinaryNode {
        Object element;
        BinaryNode *left;
        BinaryNode *right;
        int freq = 0; // counter for frequency of occurrences of element in tree

        BinaryNode(const Object &theElement, BinaryNode *lt, BinaryNode *rt)
                : element{theElement}, left{lt}, right{rt} {}

        BinaryNode(Object &&theElement, BinaryNode *lt, BinaryNode *rt)
                : element{std::move(theElement)}, left{lt}, right{rt} {}
    };

    BinaryNode *root = nullptr;
    Comparator comparator;

    void insert(const Object &x, BinaryNode *&t) {
        if (t == nullptr) {
            t = new BinaryNode(x, nullptr, nullptr);
        } else if (comparator(x, t->element)) {
            insert(x, t->left);
        } else if (t->element < x) {
            insert(comparator(x, t->right));
        } else {
            // found object x. is a duplicate. Ignore
        }
    }

    void insert(Object &&x, BinaryNode *&t) {
        if (t == nullptr) {
            t = new BinaryNode(std::move(x), nullptr, nullptr);
        } else if (comparator(x, t->element)) {
            insert(std::move(x), t->left);
        } else if (comparator(t->element, x)) {
            insert(std::move(x), t->right);
        } else {
            // duplicate.
            t->freq++;
        }
    }

    void remove(const Object &x, BinaryNode *&t) {
        if (t == nullptr) {
            return; // not found
        } else if (comparator(x, t->element)) { // i.e. c < t->element
            remove(x, t->left);
        } else if (comparator(t->element, x)) { // i.e. t-element < x
            remove(x, t->right);
        } else if (t->left != nullptr && t->right != nullptr) {
            /**
             * When both left and right are both non-null, we replace node t's data
             * with that of the lowest node from the right tree. This works because the
             * structure of the binary search tree ensures that the minimum node in the
             * right tree, from any other node is the next largest number that is smaller than
             * right value
             */
            t->element = findMin(t->right)->element;
            remove(t->element, t->right);
        } else {
            // When we have a leaf node, we take a reference to the leaf node
            // and call it oldNode. then we check whether the left is a nullptr
            // if it is then we store the value of right in t, which is a nullptr
            // when we're dealing with a leaf node.
            //
            // This code is elegant, since we also deal with the case of 1 child
            // at the same time
            //
            BinaryNode *oldNode = t;
            t = (t->left != nullptr) ? t->left : t->right;
            delete oldNode;
        }
    }

    /**
     * @brief returns true if there is a node in @param t
     * that has item @param x.
     */
    bool contains(const Object &x, BinaryNode *t) {
        // leaf nodes have a nullptr for both left and right
        // if we hit one, x is not in subtree t.
        if (t == nullptr) {
            return false;
        } else if (comparator(x, t->element)) {
            /**
             * When x is less than the value of the current T
             * call contains again with the node to the left.
             * Recall that in a binary search tree,
             * nodes are arranged such that left is less than
             * t which is less than right.
             */
            return contains(x, t->left);
        } else if (comparator(t->element, x)) {
            /**
             * When x is greater than the value of current t,
             * we call contains again to check the value right
             * against x.
             *
             * Obviously, x must implement the comparison operators.
             */
            return contains(x, t->right);
        } else {
            // if x is not less than or greater than the value of t,
            // then it is equal.
            return true;
        }
    }

    /**
     * @brief find the smallest value in the tree
     * @details keep traversing left
     */
    BinaryNode *findMin(BinaryNode *t) const {
        if (t == nullptr) {
            return nullptr;
        }
        if (t->left == nullptr) {
            return t;
        }
        return findMin(t->left);
    }

    BinaryNode *findMax(BinaryNode *t) const {
        // try a non-recursive implementation
        if (t != nullptr) {
            while (t->right != nullptr) {
                t = t->right;
            }
        }
        return t;
    }

    void makeEmpty(BinaryNode *t) {
        if (t != nullptr) {
            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
        }
        t = nullptr;
    }

    void printTree(const std::string &prefix, const BinaryNode *node, bool isLeft) {
        if (node != nullptr) {
            std::cout << prefix;

            std::cout << (isLeft ? "├──" : "└──");

            // print the value of the node
            std::cout << node->element << std::endl;

            // enter the next tree level - left and right branch
            printTree(prefix + (isLeft ? "│   " : "    "), node->left, true);
            printTree(prefix + (isLeft ? "│   " : "    "), node->right, false);
        }
    }

    BinaryNode *clone(BinaryNode *t) const {
        // when t is nullptr, we return nullptr to end the recursion
        if (t == nullptr) {
            return nullptr;
        }
        /**
         * e.g.
         *     50
         *  38    55
         * this is an elegant algorithm. We start at the root node 50. Since
         * the root node is not nullptr, we create a new BinaryNode using 50 as its
         * value. As the new root node left and right nodes, we pass in the return
         * value of another call to clone but with left or right respectively.
         * For instance, after the root node, the lt node call to clone is entered.
         * The lt is not null, so we proceed to create it. The value of left is 38
         * which is used as t->element. This time, calls to clone(left) and clone(right)
         * both return nullptr meaning they are popped off the stack and we return the the
         * frame containing the root node. We repeat the process with the right node.
         *
         * Very nice!
         */
        return new BinaryNode(t->element, clone(t->left), clone(t->right));
    }

};


class BinarySearchTreeTests : public ::testing::Test {
public:
    BinarySearchTreeTests() = default;

    /**
     * @brief create a random BST with @param N int's generated from a uniform distribution
     * between @param low and @param high.
     */
    static BinarySearchTree<int> randomBST(std::uint64_t N, int low = -100, int high = 100) {
        auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        BinarySearchTree<int> bst;
        std::default_random_engine generator(seed);

        std::uniform_int_distribution<int> u(low, high);
        for (int i = 0; i < N; i++) {
            bst.insert(u(generator));
        }
        return bst;
    }
};

TEST_F(BinarySearchTreeTests, CreateBST) {
    BinarySearchTree<int> bst;
    ASSERT_TRUE(bst.empty());
}

TEST_F(BinarySearchTreeTests, Insert) {
    BinarySearchTree<int> bst = randomBST(10);
    bst.printTree();
    ASSERT_FALSE(bst.empty());
}


TEST_F(BinarySearchTreeTests, FindMin) {
    BinarySearchTree<int> bst;
    bst.insert(5);
    bst.insert(10);
    bst.insert(15);
    bst.insert(3);
    bst.insert(7);
    auto min = bst.findMin();
    ASSERT_EQ(3, min);
}


TEST_F(BinarySearchTreeTests, FindMax) {
    BinarySearchTree<int> bst;
    bst.insert(5);
    bst.insert(10);
    bst.insert(15);
    bst.insert(3);
    bst.insert(7);
    auto max = bst.findMax();
    ASSERT_EQ(15, max);
}


TEST_F(BinarySearchTreeTests, CheckInsert) {
    BinarySearchTree<int> bst;
    bst.insert(5);
    bst.insert(10);
    bst.insert(15);
    bst.insert(3);
    bst.insert(7);
    auto max = bst.findMax();
    ASSERT_EQ(15, max);
}

TEST_F(BinarySearchTreeTests, CheckRemoveLeafNode) {
    BinarySearchTree<int> bst;
    bst.insert(5);
    bst.insert(10);
    bst.insert(15);
    bst.insert(3);
    bst.insert(7);

    bst.remove(15);

    auto max = bst.findMax();
    ASSERT_EQ(10, max);
}

TEST_F(BinarySearchTreeTests, CheckRemoveNodeThatHasOneChildLeft) {
    BinarySearchTree<int> bst;
    bst.insert(5);
    bst.insert(10);
    bst.insert(15);
    bst.insert(3);
    bst.insert(7);
    bst.insert(6);

    bst.remove(7);

    auto max = bst.findMax();
    ASSERT_EQ(10, max);
}


TEST_F(BinarySearchTreeTests, CheckRemoveNodeThatHasOneChildRight) {
    BinarySearchTree<int> bst;
    bst.insert(5);
    bst.insert(10);
    bst.insert(15);
    bst.insert(3);
    bst.insert(7);
    bst.insert(8);

    bst.remove(7);

    auto max = bst.findMax();
    ASSERT_EQ(10, max);
}

TEST_F(BinarySearchTreeTests, CheckRemoveNodeThatHasTwoChild) {
    BinarySearchTree<int> bst;
    bst.insert(50);
    bst.insert(38);
    bst.insert(55);
    bst.insert(23);
    bst.insert(16);
    bst.insert(9);
    bst.insert(12);
    bst.insert(33);
    bst.insert(26);
    bst.insert(37);
    bst.insert(36);
    bst.insert(34);

    bst.remove(33);

    auto max = bst.findMax();
}

TEST_F(BinarySearchTreeTests, Clone) {
    BinarySearchTree<int> bst;
    bst.insert(50);
    bst.insert(38);
    bst.insert(55);

    auto bst2 = bst;
}


#include <cmath>
#include <string>

bool narcissistic(int value) {
    //Code away

    // first convert value into a string so we can count digits and split them
    std::ostringstream os;
    os << value;
    std::string valAsString = os.str();

    // figure our number of digits. This is our power,
    int size = valAsString.size();

    // iterate over each of the digits and compute the sum of their values to power of size
    int total = 0;
    for (int i = 0; i < size; i++) {
        std::string numberString = std::string(1, valAsString[i]);

        int number = std::stoi(numberString);
        std::cout << number << std::endl;
        total += pow((double) number, (double) size);
    }
    return value == total;
}

TEST_F(BinarySearchTreeTests, t) {
    std::cout << narcissistic(7) << std::endl;


}


bool isTriangle(int a, int b, int c) {
    if (a <= 0 || b <= 0 || c <= 0) {
        return false;
    }

    if (a == b && a == c)
        return true;
    /*
    Inequality Triangle Theorem
    The sum of the lengths of any two sides of a triangle is
    greater than the length of the third side.
    */
    std::cout << std::numeric_limits<int>::max() << std::endl;
    bool AAndBGTC = (static_cast<std::uint64_t>(a) + static_cast<std::uint64_t >(b)) > static_cast<std::uint64_t >(c);
    bool BAndCGTA = (static_cast<std::uint64_t>(b) + static_cast<std::uint64_t >(c)) > static_cast<std::uint64_t >(a);
    bool AAndCGTB = (static_cast<std::uint64_t>(a) + static_cast<std::uint64_t >(c)) > static_cast<std::uint64_t >(b);

    return AAndBGTC && BAndCGTA && AAndCGTB;
}


TEST(isTriangleTests, CheckThreeBigEqualNumbers){
    bool ans = isTriangle(2147483647,2147483647,2147483647);
    std::cout << ans << std::endl;
    ASSERT_TRUE(ans);
}
TEST(isTriangleTests, CheckTwoBigEqualNumbersOneSmall){
    bool ans = isTriangle(10,2147483647,2147483647);
    std::cout << ans << std::endl;
    ASSERT_TRUE(ans);
}


class Accumul
{
public:
    static std::string accum(const std::string &s);
};

std::string Accumul::accum(const std::string& s){
  // counter for current letter
  int counter = 0;
  std::ostringstream output;
  for (auto c: s){
    for (int i=0; i<=counter; i++){
      if (i==0)
        output << (char)std::toupper(c);
      else
        output << c;
    }
    counter++;
    std::cout << *--s.end() << std::endl;
    if (c != *s.end())
      output << "-";
  }

  return output.str();

}


TEST(words, w){
    std::string actual = Accumul::accum("ZpglnRxqenU");
    std::string expected = "Z-Pp-Ggg-Llll-Nnnnn-Rrrrrr-Xxxxxxx-Qqqqqqqq-Eeeeeeeee-Nnnnnnnnnn-Uuuuuuuuuuu";
    ASSERT_STREQ(expected.c_str(), actual.c_str());
}

TEST(words, OneLetter){
    std::string actual = Accumul::accum("Z");
    std::string expected = "Z";
    ASSERT_STREQ(expected.c_str(), actual.c_str());
}










