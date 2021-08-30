//
// Created by Ciaran on 26/08/2021.
//

#ifndef CRACKINGTHECODINGINTERVIEW_LIST_H
#define CRACKINGTHECODINGINTERVIEW_LIST_H

template <typename T>
class List {
public:
    List() = default;

    virtual void append(T item) = 0;

    virtual int size() = 0;

    virtual T get(int idx) = 0;

    virtual void set(int idx, T value) = 0;

//    get
//    set
//    appemd
//    insert
//    delete
//    size

protected:
    /**
     * Variable for keeping track of the number
     * of items in the list
     */
    int count = 0;

};

#endif //CRACKINGTHECODINGINTERVIEW_LIST_H
