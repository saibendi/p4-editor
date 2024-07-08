#ifndef LIST_HPP
#define LIST_HPP
/* List.hpp
 *
 * doubly-linked, double-ended list with Iterator interface
 * EECS 280 Project 4
 */

#include <iostream>
#include <iterator> //std::bidirectional_iterator_tag
#include <cassert>  //assert


template <typename T>
class List {
  //OVERVIEW: a doubly-linked, double-ended list with Iterator interface
public:

  //EFFECTS:  returns true if the list is empty
    bool empty() const {
        // if first ptr and last ptr are both null then list doesn't have any nodes
        return (first == nullptr && last == nullptr);
    }

  //EFFECTS: returns the number of elements in this List
  //HINT:    Traversing a list is really slow. Instead, keep track of the size
  //         with a private member variable. That's how std::list does it.
    int size() const {
        return list_size;
    }

  //REQUIRES: list is not empty
  //EFFECTS: Returns the first element in the list by reference
    T & front() {
        assert(!empty());
        return first->datum;
    }

  //REQUIRES: list is not empty
  //EFFECTS: Returns the last element in the list by reference
    T & back() {
        assert(!empty());
        return last->datum;
    }

  //EFFECTS:  inserts datum into the front of the list
    void push_front(const T &datum) {
        Node *p = new Node;
        p->datum = datum;
        if (empty()) {
            first = last = p;
            p->next = nullptr;
            p->prev = nullptr;
        }
        else {
            p->next = first;
            p->prev = nullptr;
            first->prev = p;
            first = p;  // first ptr points to the new Node now
        }
        ++list_size; // increment size whenever you add new Node
    }

  //EFFECTS:  inserts datum into the back of the list
    void push_back(const T &datum) {
        Node *p = new Node;
        p->datum = datum;
        p->next = nullptr;
        if (empty()) {
            last = first = p;
            p->next = nullptr;
            p->prev = nullptr;
        }
        else {
            p->next = nullptr;
            p->prev = last;
            last->next = p;
            last = p;  // last ptr points to the new Node now
        }
        ++list_size;
    }

  //REQUIRES: list is not empty
  //MODIFIES: may invalidate list iterators
  //EFFECTS:  removes the item at the front of the list
    void pop_front() {
        //previous implementation doesn't take into account what to do with last when the list is empty
        assert(!empty());
        Node* victim = first;
        if (first == last) {
            first = last = nullptr;
        } else {
            first = first->next;
            first->prev = nullptr;
        }
        delete victim;
        --list_size;
    }

  //REQUIRES: list is not empty
  //MODIFIES: may invalidate list iterators
  //EFFECTS:  removes the item at the back of the list
    void pop_back() {
        //previous implementation doesn't take into account what to do with last when the list is empty
        assert(!empty());
        Node* victim = last;
        if (first == last) {
            first = last = nullptr;
        } else {
            last = last->prev;
            last->next = nullptr;
        }
        delete victim;
        --list_size;
    }

  //MODIFIES: may invalidate list iterators
  //EFFECTS:  removes all items from the list
    void clear() {
        while(!empty()) {
            pop_front();
        }
    }

  // You should add in a default constructor, destructor, copy constructor,
  // and overloaded assignment operator, if appropriate. If these operations
  // will work correctly without defining these, you should omit them. A user
  // of the class must be able to create, copy, assign, and destroy Lists.

    // default ctor
    List() : list_size(0), first(nullptr), last(nullptr) {}
    
    
    // copy ctor
    List(const List<T> &other) : list_size(0), first(nullptr), last(nullptr) {
        copy_all(other);
    }

    // assignment operator
    List& operator=(const List<T> &rhs) {
        if (this == &rhs) {  // preventing self-assignment
            return *this;
        }
        clear();
        copy_all(rhs);
        return *this;
    }

    // dtor
    ~List() {
        clear();
    }

    
private:
  //a private type
  struct Node {
    Node *next;
    Node *prev;
    T datum;
  };

  // size of the list for size() function
  int list_size;
    
  //REQUIRES: list is empty
  //EFFECTS:  copies all nodes from other to this
    void copy_all(const List<T> &other) {
        // first need to assert that this list is empty
        assert(empty());
        for (Node *ptr = other.first; ptr != nullptr; ptr = ptr->next) {
            push_back(ptr->datum);
        }
    }

  Node *first;   // points to first Node in list, or nullptr if list is empty
  Node *last;    // points to last Node in list, or nullptr if list is empty

public:
  ////////////////////////////////////////
  class Iterator {
  public:
    //OVERVIEW: Iterator interface to List

    // Add a default constructor here. The default constructor must set both
    // pointer members to null pointers.
      Iterator() : list_ptr(nullptr), node_ptr(nullptr) {};


    // Add custom implementations of the destructor, copy constructor, and
    // overloaded assignment operator, if appropriate. If these operations
    // will work correctly without defining these, you should omit them. A user
    // of the class must be able to copy, assign, and destroy Iterators.

      // TODO: copy ctor
      Iterator(const Iterator& other) : list_ptr(other.list_ptr), node_ptr(other.node_ptr) {}
      
      
      // TODO: assignment operator
      Iterator operator=(const Iterator& rhs) {
          list_ptr = rhs.list_ptr;
          node_ptr = rhs.node_ptr;
          return *this;
      }
      
      // TODO: dtor
      ~Iterator() {
          list_ptr = nullptr;
          node_ptr = nullptr;
      }

    // Your iterator should implement the following public operators:
    // *, ++ (both prefix and postfix), == and !=.
    // Equality comparisons must satisfy the following rules:
    // - two default-constructed iterators must compare equal
    // - a default-constructed iterator must compare unequal to an
    //   iterator obtained from a list, even if it is the end iterator
    // - two iterators to the same location in the same list must
    //   compare equal
    // - two iterators to different locations in the same list must
    //   compare unequal
    // - comparing iterators obtained from different lists results in
    //   undefined behavior
    //   - Your implementation can handle this however you like, but
    //     don't assume any particular behavior in your test cases --
    //     comparing iterators from different lists essentially
    //     violates the REQURES clause.
    // Note: comparing both the list and node pointers should be
    // sufficient to meet these requirements.
      
      T& operator* () const {
          // TODO: can there be a scenario where node_ptr is nullptr, but list_ptr isn't?
          assert(list_ptr != nullptr && node_ptr != nullptr);
          return node_ptr->datum;
      }
      
      Iterator& operator++ () {
          // TODO: can there be a scenario where node_ptr is nullptr, but list_ptr isn't?
          // removed  node_ptr != nullptr in assert condition bc we need to be able to go to the nullptr location as well
          assert(list_ptr);
          assert(*this != list_ptr->end());
          if (node_ptr) {
              node_ptr = node_ptr->next;
          }
          return *this;
      }
      
      bool operator== (const Iterator& rhs) const {
          return node_ptr == rhs.node_ptr;
      }

      bool operator!= (const Iterator& rhs) const {
          return node_ptr != rhs.node_ptr;
      }

    // Type aliases required to work with STL algorithms. Do not modify these.
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    // This operator will be used to test your code. Do not modify it.
    // REQUIRES: Iterator is decrementable. All iterators associated with a
    //           list are decrementable, including end iterators, except for
    //           begin iterators (those equal to begin() on the list)
    // EFFECTS:  moves this Iterator to point to the previous element
    //           and returns a reference to this Iterator
    Iterator& operator--() { // prefix -- (e.g. --it)
      assert(list_ptr);
      assert(*this != list_ptr->begin());
      if (node_ptr) {
        node_ptr = node_ptr->prev;
      } else { // decrementing an end Iterator moves it to the last element
        node_ptr = list_ptr->last;
      }
      return *this;
    }

    // This operator will be used to test your code. Do not modify it.
    // REQUIRES: Iterator is decrementable. All iterators associated with a
    //           list are decrementable, including end iterators, except for
    //           begin iterators (those equal to begin() on the list)
    // EFFECTS:  moves this Iterator to point to the previous element
    //           and returns a copy of the original Iterator
    Iterator operator--(int ) { // postfix -- (e.g. it--)
      Iterator copy = *this;
      operator--();
      return copy;
    }

    // REQUIRES: Iterator is dereferenceable
    // EFFECTS: returns the underlying element by pointer
    // NOTE: T must be a class type to use the syntax it->. If T has a
    //       member variable f, then it->f accesses f on the
    //       underlying T element.
    T* operator->() const {
      return &operator*();
    }

  private:
    const List *list_ptr; //pointer to the List associated with this Iterator
    Node *node_ptr; //current Iterator position is a List node
    // add any additional necessary member variables here


    // add any friend declarations here
      friend class List;

    // construct an Iterator at a specific position in the given List
      Iterator(const List *lp, Node *np) : list_ptr (lp), node_ptr (np) {}

  };//List::Iterator
  ////////////////////////////////////////

  // return an Iterator pointing to the first element
    Iterator begin() const {
        return Iterator(this, first);
    }

  // return an Iterator pointing to "past the end"
    Iterator end() const {
        return Iterator(this, nullptr);
    }

  //REQUIRES: i is a valid, dereferenceable iterator associated with this list
  //MODIFIES: may invalidate other list iterators
  //EFFECTS: Removes a single element from the list container.
  //         Returns An iterator pointing to the element that followed the
  //         element erased by the function call
    Iterator erase(Iterator i) {
        // asserting that list pointer points to this list
        assert(i.list_ptr == this);
        // asserting that node ptr isn't null
        assert(i.node_ptr != nullptr);
        
        Node *temp = i.node_ptr;
        if (temp == first) {
            pop_front();
            return begin(); // returns element that followed what we removed, which now becomes first in List
        }
        else if (temp == last) {
            pop_back();
            return end();   // returns element past the last node
        }
        else {
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            i.node_ptr = i.node_ptr->next;
            delete temp;
            --list_size;
            return i;      // i now points to the next ptr 
        }
    }

  //REQUIRES: i is a valid iterator associated with this list
  //EFFECTS: Inserts datum before the element at the specified position.
  //         Returns an iterator to the the newly inserted element.
    Iterator insert(Iterator i, const T &datum) {
        // asserting that list pointer points to this list
        assert(i.list_ptr == this);

        Node *temp = i.node_ptr;
        if (temp == first) { // if i is at the front of the list
            push_front(datum);
            return begin();
        }
        else if (temp == nullptr) { // if i is at the back of the list
            push_back(datum);
            return Iterator(this, last);
        }
        else {  // if i is somewhere in the middle of the list
            Node *new_node = new Node;
            new_node->datum = datum;
            new_node->prev = temp->prev;
            new_node->next = temp;
            temp->prev = new_node;
            temp->prev->next = new_node;
            ++list_size;
            return Iterator(this, new_node);
        }
    }
};//List


////////////////////////////////////////////////////////////////////////////////
// Add your member function implementations below or in the class above
// (your choice). Do not change the public interface of List, although you
// may add the Big Three if needed.  Do add the public member functions for
// Iterator.


#endif // Do not remove this. Write all your code above this line.
