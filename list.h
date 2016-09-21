// list.h
// Alistair Campbell
// Fall 2009
// Spring 2011
// Fall 2011
// Spring 2012
// Fall 2012
// Spring 2013
// Fall 2014
// Revised spring 2015

/*******************************

Assignment:  Implement the data structure described herein.

 *******************************/

#ifndef LIST
#define LIST

#include <iostream>

typedef int list_element;  // change this if you want a list of something other
                          // than int.

struct list_node { // base class for list nodes
  list_node* prev;
  list_element data;
  list_node* next;
  list_node();
  list_node(list_node* p,list_node* n);  // create node with given prev 
                                      // and next pointers
  list_node(list_node* p, const list_element & d, list_node* n);  // or all 3.
};

class list 
{
 public:
  list();
  // Create an empty doubly-linked list with front and rear sentinels,
  // current points to rear, and current_index = 0;

  ~list();
  // Delete all nodes (including the sentinels)

  list(const list & orig);
  // Make a (deep) copy of a list.
  // current_index and current can be set arbitrarily in the copy

  list & operator=(const list & rhs);
  // Support list assignment, with deep copy.  Similarly, current_index
  // and current are set arbitrarily.

  void add(const list_element & item,int index);
  // Insert a new element at a position (k) given by index.
  //     If index is non-negative, k is index; otherwise
  //     k is size() + index.
  // PRE:  this list contains <v0, v1, ..., vk, ...>
  //       0 <= k <= size()
  // POST: this list contains <v0, v1, ..., item, vk, ... >
  //       current points to new node containing item.
  //       current_index is k.
  // Guarantees constant time access to either end or vicinity of
  // current_index.  

  void remove_at(int index);
  // Remove an element from this list.  
  // index indicates a position k in the manner described in (add).
  // Pre: 0 <= k < size()
  //      This list contains <v0, v1, ..., vk, v(k+1), ... >
  // Post: This list contains <v0, v1, ..., v(k+1), ... >
  //       current points to v(k+1) (or the rear sentinel)
  // Guarantees constant time access to either end or vicinity of
  // current_index.  

  void remove(const list_element & item);
  // Let i be the smallest non-negative integer where get(i)==item. 
  // Post: same as for removeAt(i)
  // If no such i exists, no change to the list.

  size_t find(const list_element & item) const;
  // return smallest non-negative integer where get(i)==item,
  // or size() if no such integer

  list_element get(int index) const;
  // index indicates a position k in the manner described in (add).
  // return the element at position k.
  // Guarantees constant time access to either end or vicinity of
  // current_index.  

  size_t size() const;
  // return the number of elements of the list
  // runs in constant time
  
  void output(std::ostream & ostr) const;
  // output the list to ostr using format
  // <v0, v1, v2, ..., vn-1>

 private:

  void _set_current_index(size_t index) const;  
  // PRE: 0 <= index <= size()
  // POST: (1) current points to the node holding the element at position 
  //       index, or to the rear sentinel.
  //       (2) current_index is index
  // This is accomplished as quickly as possible.  (One of front, rear,
  // or current is closest to index.)

  // data fields:
  size_t _size; // number of data items
  list_node* _front;  // points to front sentinel
  list_node* _rear;   // points to rear sentinel
  mutable list_node* _current;  // points to a node in the list beyond the 
                          // front sentinel
  mutable size_t _current_index;  // corresponds to current.  
                        // 0 <= current_index <= size()
  void _init();
  void _copy(const list & orig);
  void _destroy();
    
};

#endif
