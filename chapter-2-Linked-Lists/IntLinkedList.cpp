//
// Created by Ciaran on 15/08/2021.
//
#include <iostream>
#include <sstream>

#include "gtest/gtest.h"

/**
 * @brief a node in a singly linked list
 */
class Node {
public:
    Node(int value)
            : value_(value) {}

    /**
     * @brief destructor, traverses the linked
     * list until it finds nullptr. If
     * the node is not a nullptr, then delete it.
     */
    ~Node() {
        while (next != nullptr) {
            if (next) {
                delete next;
                next = nullptr;
            }
        }
    }

    /**
     * Append a new value to the end of the singly
     * linked list
     */
    void append(int value) {
        Node* thisNode = this;
        Node *endNode = new Node(value);
        while (next != nullptr) {
            std::cout << value_ << std::endl;
            this->next = next->next;
        }
        next = endNode;
    }

    /**
     * @brief remove a node from the singly linked list based on its value
     */
    void removeValue(int value) {
        while (next != nullptr) {
            if (next->next->value_ == value) {
                // perform deleting operation
                next = next->next; // reassign next to two nodes
            }
            delete next->next;
        }
    }

    bool operator==(const Node &other) {
        return value_ == other.value_;
    }

    bool operator!=(const Node &other) {
        return !(*this == other);
    }

    /**
     * @brief output to stream
     */
    friend std::ostream &operator<<(std::ostream &os, Node &node) {
        os << "[";
        os << node.value_;
        while (node.next != nullptr) {
            os << ", " << node.next->value_;
            node.next = node.next->next;
        }
        os << "]";
        return os;
    }

private:
    Node *next = nullptr;
    int value_;
};


class SinglyLinkedList : public ::testing::Test {
public:
    SinglyLinkedList() = default;
};

TEST_F(SinglyLinkedList, CreateLL){

}

//int main() {
//    Node n(4);
//    std::cout << n << std::endl;
//
//    n.append(6);
//    std::cout << n << std::endl;
//    n.append(8);
//    std::cout << n << std::endl;
//    n.append(9);
//    std::cout << n << std::endl;
//
//
//}
//
