//
// Created by Ciaran on 26/08/2021.
//

#include "gtest/gtest.h"
#include "List.h"

struct node {
    explicit node(int d) : data(d){}
    int data;
    std::shared_ptr<node> next = nullptr;
};

using SharedNodePtr = std::shared_ptr<node>;

class SinglyLinkedList : public List<int> {
public:
    SinglyLinkedList() = default;

    void append(int item) override{
        auto newNode = std::make_shared<node>(item);
        if (count == 0){
            head = newNode;
            begin() = newNode;
        } else {
            head->next = newNode;
        }
        count++;
    }

    SharedNodePtr begin(){
        return firstNode;
    }

    SharedNodePtr end(){
        return endNode;
    }


    int size(){
        return count;
    }

    int get(int idx) override {
        if (idx > count-1){
            throw std::invalid_argument("idx is greater than the size of this List");
        }

        int i = 0;
        while (i < idx){
            head = head->next;
            i++;
        }
        int data = head->data;
//        head = begin();
        return data;
    }

    void set(int idx, int value) override{

    }

    friend std::ostream & operator<<(std::ostream& os, SinglyLinkedList linkedList){
        os << "[";
        SharedNodePtr startNode = linkedList.begin();
        while (startNode->next != nullptr){
            os << linkedList.head->data << ", ";
        }
        os << "]";
        return os;
    }


private:
    std::shared_ptr<node> head = nullptr;
    std::shared_ptr<node> tail = nullptr;
    std::shared_ptr<node> firstNode = nullptr;
    std::shared_ptr<node> endNode = nullptr;
};


class SinglyLinkedListTests : public ::testing::Test {
public:
    SinglyLinkedListTests() = default;
};

TEST_F(SinglyLinkedListTests, CheckAppendAndGet) {
    SinglyLinkedList singlyLinkedList;
    singlyLinkedList.append(4);
    ASSERT_EQ(4, singlyLinkedList.get(0));
}

TEST_F(SinglyLinkedListTests, CheckThrowsIdxError) {
    SinglyLinkedList singlyLinkedList;
    singlyLinkedList.append(4);
    ASSERT_THROW(singlyLinkedList.get(1), std::invalid_argument);
}

TEST_F(SinglyLinkedListTests, StoreTwoValues) {
    SinglyLinkedList singlyLinkedList;
    singlyLinkedList.append(4);
    singlyLinkedList.append(6);
    singlyLinkedList.append(8);
    auto actual =singlyLinkedList.get(1);
    std::cout << singlyLinkedList << std::endl;
//    ASSERT_EQ(6, actual);
}














