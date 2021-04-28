/*
    LinkedList.h

    Contains the LinkedList function implementation.
*/

#define DEBUG

#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <sstream>
using std::out_of_range;
using std::ostream;
using std::cout;
using std::stringstream;

#ifdef DEBUG
#include <vector>
using std::vector;
#endif

template <typename T>
class LinkedList {
private:
    class Node {
    public:
        Node() : next(NULL), prev(NULL) {}
        Node(T sentData) : data(sentData), next(NULL), prev(NULL) {}
        T data;
        Node *next;
        Node *prev;
    };
    Node *head;
    Node *tail;
    unsigned int count;
public:
    LinkedList();
    LinkedList(const LinkedList& lst);
    ~LinkedList();
    void clear();
    unsigned int size() const;
    bool empty() const;
    LinkedList<T>& push_front(T data);
    LinkedList<T>& push_back(T data);
    LinkedList<T>& push(T data);
    T pop_front();
    T pop_back();
    T pop();
    T peek_front();
    T peek_back();
    T peek();

    void insertAt(unsigned int pos, T data);
    void deleteAt(unsigned int pos);
    bool remove(T data);
    bool contains(T data);
    T& operator[](const unsigned int pos);

#ifdef DEBUG
    static bool test();
    static bool check(const vector<int>& v, const LinkedList<int>& lst);
#endif

    friend ostream& operator<<(ostream& strm, const LinkedList<T>& lst) {
        Node *p = lst.head;
        strm << "[";
        while (p != NULL) {
            strm << p -> data;
            p = p -> next;
            if (p != NULL) strm << ",";
        }
        strm << "]";
        return strm;
    }
};

// Default constructor
template <typename T>
LinkedList<T>::LinkedList() : head(NULL), tail(NULL), count(0) { }

// Copy constructor
template <typename T>
LinkedList<T>::LinkedList(const LinkedList& lst) {
    if (lst.head == NULL) {
        head = tail = NULL;
        count = 0;
    } else {
        head = new Node(lst.head->data);

        Node *ptrCurr = head;
        Node *ptrOld = lst.head->next;
        while (ptrOld != NULL) {
            ptrCurr->next = new Node(ptrOld->data);
            // *** Need to set the next node's prev ptr to current node
            ptrCurr->next->prev = ptrCurr;
            ptrCurr = ptrCurr->next;
            ptrOld = ptrOld->next;
        }
        ptrCurr->next = NULL;
        tail = ptrCurr;
        count = lst.count;
    }
}

// Destructor
template <typename T>
LinkedList<T>::~LinkedList() {
    clear();
}

// Remove all items from list
template <typename T>
void LinkedList<T>::clear() {
    Node *p = head;
    while (head != NULL) {
        p = head;
        head = head->next;
        delete p;
    }
    tail = NULL;
    count = 0;
}

// Return number of items in list
template <typename T>
unsigned int LinkedList<T>::size() const {
    return count;
}

// Return true if list has no items
template <typename T>
bool LinkedList<T>::empty() const {
    return count == 0;
}

// Add an item to the front of the list
template <typename T>
LinkedList<T>& LinkedList<T>::push_front(T data) {
    Node *newNode = new Node(data);
    // *** Need to set the new node's prev ptr to NULL
    newNode->prev = NULL;
    if (head != NULL) head->prev = newNode;// *** then set the old head's prev ptr to the new node
    else tail = newNode;
    newNode->next = head;
    head = newNode;
    count++;
    return *this;
}

// Add an item to the back of the list
template <typename T>
LinkedList<T>& LinkedList<T>::push_back(T data) {
    Node *newNode = new Node(data);
    newNode->next = NULL;
    if (tail != NULL) tail->next = newNode;
    else head = newNode;
    // *** Need to set the new node's prev ptr to what used to be the tail node
    newNode->prev = tail;
    tail = newNode;
    count++;
    return *this;
}

// Add an item to the list (alias for push_back)
template <typename T>
LinkedList<T>& LinkedList<T>::push(T data) {
    return push_back(data);
}

// Remove an item from the front of the list
template <typename T>
T LinkedList<T>::pop_front() {
    if (count == 0) throw out_of_range("Attempt to pop from empty list");
    Node *temp = head;
    T data;
    head = head->next;
    if (head != NULL) head->prev = NULL; // *** then set head's prev ptr to NULL
    else tail = NULL;
    data = temp->data;
    delete temp;
    count--;
    return data;
}

// Remove an item from the back of the list
template <typename T>
T LinkedList<T>::pop_back() {
    if (count == 0) throw out_of_range("Attempt to pop from empty list");
    Node *temp = tail;
    T data;
    // *** Need to reset tail to point to the prev list node
    tail = tail->prev;
    if (tail != NULL) tail->next = NULL;
    else head = NULL;
    data = temp->data;
    delete temp;
    count--;
    return data;
}

// Remove an item from (the back of) the list
template <typename T>
T LinkedList<T>::pop() {
    return pop_back();
}

// Return data from first list element
template <typename T>
T LinkedList<T>::peek_front() {
    if (count == 0) throw out_of_range("Attempt to peek at empty list");
    return head->data;
}

// Return data from last list element
template <typename T>
T LinkedList<T>::peek_back() {
    if (count == 0) throw out_of_range("Attempt to peek at empty list");
    return tail->data;
}

// Return data from last list element
template <typename T>
T LinkedList<T>::peek() {
    return peek_back();
}

// Insert an item at a specified position in the list
template <typename T>
void LinkedList<T>::insertAt(unsigned int pos, T data) {
    if (pos > count) throw out_of_range("Attempt to insert beyond bounds");
    Node *pCurr;
    Node *newNode;

    newNode = new Node(data);
    if (pos == 0) {
        if (head != NULL) head->prev = newNode; // *** then set the old head's prev ptr to the new node
        else tail = newNode;
        newNode->next = head;
        head = newNode;
    } else if (pos == count) {
        tail->next = newNode;
        // *** Need to set the new node's prev ptr to the current tail
        newNode->prev = tail;
        tail = newNode;
    } else {
        pCurr = head;
        for (unsigned int i = 0; i < pos-1; i++) {
            assert (pCurr != NULL);
            pCurr = pCurr->next;
        }
        newNode->next = pCurr->next;
        if (pCurr->next != NULL) pCurr->next->prev = newNode; // *** then set the next node's prev ptr to the new node
        // Need to set the new node's prev ptr to the current node
        newNode->prev = pCurr;
        pCurr->next = newNode;
    }
    count++;
}

// Delete an item at a specified position
template <typename T>
void LinkedList<T>::deleteAt(unsigned int pos) {
    if (pos < 0 || pos >= count) throw out_of_range("Attempt to delete beyond bounds");
    Node *pCurr;
    Node *temp;
    if (pos == 0) pop_front();
    else if (pos == count-1) pop_back();
    else {
        pCurr = head;
        for (unsigned int i = 0; i < pos-1; i++) {
            assert (pCurr != NULL);
            pCurr = pCurr->next;
        }
        temp = pCurr->next;
        pCurr->next = temp->next;
        // *** Need to set the prev ptr of the node after temp to the current node
        temp->next->prev = pCurr;
        delete temp;
        count--;
    }
}

// Delete a particular value from list
// Returns true or false based on whether value was found.
template <typename T>
bool LinkedList<T>::remove(T data) {
    Node *pCurr = head;

    while (pCurr != NULL && pCurr->data != data) {
        pCurr = pCurr->next;
    }

    if (pCurr == NULL) return false;
    if (head == pCurr) head = pCurr->next;
    else pCurr->prev->next = pCurr->next; // *** Need to set the previous node's next ptr to whatever the current node points to as next
    if (tail == pCurr) tail = pCurr->prev; // *** then set tail to be whatever node comes before the current node
    else pCurr->next->prev = pCurr->prev; // *** then set the next node's prev ptr to whatever comes before the current node
    delete pCurr;
    count--;
    return true;
}

// Find out if the list contains a particular value
// Returns true or false based on whether value was found.
template <typename T>
bool LinkedList<T>::contains(T data) {
    Node *pCurr = head;

    while (pCurr != NULL) {
        if (pCurr->data == data) return true;
        pCurr = pCurr->next;
    }
    return false;
}

// Overload of [] operator
template <typename T>
T& LinkedList<T>::operator[](const unsigned int pos) {
    if (pos >= count) {
        stringstream strm;
        strm << "Attempted access beyond bounds [" << pos << "]";
        throw out_of_range(strm.str());
    }
    Node *p = head;
    unsigned int n = 0;
    while (n < pos) {
        p = p->next;
        n++;
    }
    return p->data;
}

#ifdef DEBUG
template <typename T>
bool LinkedList<T>::test() {
    int errors = 0;
    int tests = 0;

    LinkedList<int> lst;
    vector<int> v;

    // Test default constructor
    tests++;
    if (!check(v, lst)) {
        cout << "Error: default constructor\n";
        errors++;
    } else {
        cout << "Passed: default constructor test\n";
    }

    // Test push_front(T) function
    lst.push_front(2).push_front(1);
    v.push_back(1);
    v.push_back(2);
    tests++;
    if (!check(v, lst)) {
        cout << "Error: push_front(int)\n";
        errors++;
    } else {
        cout << "Passed: push_front(int) test\n";
    }

    // Test push_back(T) function
    lst.push_back(3);
    v.push_back(3);
    tests++;
    if (!check(v, lst)) {
        cout << "Error: push_back(int)\n";
        errors++;
    } else {
        cout << "Passed: push_back(int) test\n";
    }

    // Test push(T) function
    lst.push(4);
    v.push_back(4);
    tests++;
    if (!check(v, lst)) {
        cout << "Error: push(int)\n";
        errors++;
    } else {
        cout << "Passed: push(int) test\n";
    }

    // Test peek_front() function
    tests++;
    if (lst.peek_front() != v[0]) {
        cout << "Error: peek_front()\n";
        errors++;
    } else {
        cout << "Passed: peek_front() test\n";
    }

    // Test peek_back() function
    tests++;
    if (lst.peek_back() != v[v.size()-1]) {
        cout << "Error: peek_back()\n";
        errors++;
    } else {
        cout << "Passed: peek_back() test\n";
    }

    // Test peek() function
    tests++;
    if (lst.peek() != v[v.size()-1]) {
        cout << "Error: peek()\n";
        errors++;
    } else {
        cout << "Passed: peek() test\n";
    }

    // Test insert_at(int, T) function
    tests++;
    lst.insertAt(0, 11);
    lst.insertAt(2, 12);
    lst.insertAt(lst.size(), 13);
    v.insert(v.begin(), 11);
    v.insert(v.begin()+2, 12);
    v.insert(v.end(), 13);
    if (!check(v, lst)) {
        cout << "Error: insertAt(int, int)\n";
        errors++;
        cout << "Is:  ";
        for (unsigned int i=0; i<lst.size(); i++) cout << lst[i] << ' ';
        cout << "\nS/b: ";
        for (unsigned int i=0; i<v.size(); i++) cout << v[i] << ' ';
        cout << '\n';
    } else {
        cout << "Passed: insertAt(int, int) test\n";
    }

    // Test delete_at(int) function
    tests++;
    v.erase(v.begin());
    v.erase(v.begin()+1);
    v.erase(v.begin()+(v.size()-1));
    lst.deleteAt(0);
    lst.deleteAt(1);
    lst.deleteAt(lst.size()-1);
    if (!check(v, lst)) {
        cout << "Error: deleteAt(int, int)\n";
        errors++;
        cout << "Is:  ";
        for (unsigned int i=0; i<lst.size(); i++) cout << lst[i] << ' ';
        cout << "\nS/b: ";
        for (unsigned int i=0; i<v.size(); i++) cout << v[i] << ' ';
        cout << '\n';
    } else {
        cout << "Passed: deleteAt(int, int) test\n";
    }

    // Test remove(T) function
    tests++;
    v.erase(v.begin()+1);
    v.erase(v.begin());
    v.erase(v.begin()+(v.size()-1));
    lst.remove(2);
    lst.remove(4);
    lst.remove(1);
    if (!check(v, lst)) {
        cout << "Error: remove(int)\n";
        errors++;
        cout << "Is:  ";
        for (unsigned int i=0; i<lst.size(); i++) cout << lst[i] << ' ';
        cout << "\nS/b: ";
        for (unsigned int i=0; i<v.size(); i++) cout << v[i] << ' ';
        cout << '\n';
    } else {
        cout << "Passed: remove(int) test\n";
    }

    // Test subscript operator overload
    tests++;
    v[0] = 5;
    v.push_back(10);
    v.push_back(15);
    lst.push(1);
    lst.push(1);
    lst[0] = 5;
    lst[1] = lst[0] + lst[0];
    lst[2] = lst[0] + lst[1];
    if (!check(v, lst)) {
        cout << "Error: operator[]\n";
        errors++;
        cout << "Is:  ";
        for (unsigned int i=0; i<lst.size(); i++) cout << lst[i] << ' ';
        cout << "\nS/b: ";
        for (unsigned int i=0; i<v.size(); i++) cout << v[i] << ' ';
        cout << '\n';
    } else {
        cout << "Passed: operator[] test\n";
    }

    // Test copy constructor
    tests++;
    LinkedList<int> lst2(lst);
    if (!check(v, lst2)) {
        cout << "Error: copy constructor\n";
        errors++;
        cout << "Is:  ";
        for (unsigned int i=0; i<lst2.size(); i++) cout << lst[i] << ' ';
        cout << "\nS/b: ";
        for (unsigned int i=0; i<v.size(); i++) cout << v[i] << ' ';
        cout << '\n';
    } else {
        cout << "Passed: copy constructor test\n";
    }

    // Test pop() function
    tests++;
    lst.push_back(20);
    lst.push_back(25);
    if (lst.pop() != 25 || lst.size() != lst2.size()+1) {
        cout << "Error: pop()\n";
        errors++;
    } else {
        cout << "Passed: pop() test\n";
    }

    // Test pop_back() function
    tests++;
    if (lst.pop_back() != 20 || lst.size() != lst2.size()) {
        cout << "Error: pop_back()\n";
        errors++;
    } else {
        cout << "Passed: pop_back() test\n";
    }

    // Test pop_front() function
    tests++;
    if (lst.pop_front() != 5 || lst.size() != lst2.size()-1) {
        cout << "Error: pop_front()\n";
        errors++;
    } else {
        cout << "Passed: pop_front() test\n";
    }

    // Additional test for pop*() functions
    v.clear();
    v.push_back(10);
    v.push_back(15);
    tests++;
    if (!check(v, lst)) {
        cout << "Error: pop*\n";
        errors++;
    } else {
        cout << "Passed: pop* test\n";
    }

    // Test contains(T) function
    lst.push_back(20);
    tests++;
    if (!lst.contains(10) || !lst.contains(15) || !lst.contains(20) || lst.contains(5)) {
        cout << "Error: contains()\n";
        errors++;
    } else {
        cout << "Passed: contains()\n";
    }

    // Test clear() function
    lst.clear();
    tests++;
    if (lst.size() != 0 || lst2.size() == 0 || lst.head != NULL || lst.tail != NULL) {
        cout << "Error: clear()\n";
        errors++;
    } else {
        cout << "Passed: clear() test\n";
    }

    // Test empty() function
    tests++;
    if (!lst.empty()) {
        cout << "Error: empty()\n";
        errors++;
    } else {
        cout << "Passed: empty() test\n";
    }

    cout << tests << " tests attempted\n";
    cout << errors << " errors encountered\n";
    return errors == 0;
}

template <typename T>
bool LinkedList<T>::check(const vector<int>& v, const LinkedList<int>& lst) {
    if ((v.size() != lst.size()) || (lst.size() != lst.count)) return false;
    if (lst.count == 0 && (lst.head != NULL || lst.tail != NULL)) return false;
    if (lst.count != 0 && (lst.head == NULL || lst.tail == NULL)) return false;
    Node* p = lst.head;
    for (unsigned int i=0; i< lst.count; i++) {
        if (p == NULL || p->data != v[i]) return false;
        // *** Uncomment the next line when prev pointers exist
        if (i == 0 && (lst.head != p || p->prev != NULL)) return false;
        if (i == lst.count-1 && (lst.tail != p || p->next != NULL)) return false;
        p = p->next;
    }
    return true;
}

#endif
#endif
