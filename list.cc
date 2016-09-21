/*############################################################################
#                                                                            #
#  list.cc                                                                   #
#                                                                            #
#  Complete by: Oliver Keh                                                   #
#                                                                            #
#  March 11, 2016                                                            #
#                                                                            #
#  This file manipulates elements of data in this linked list by adding and  #
#  removing nodes at certain positions. It reaches those nodes by accessing  #
#  indices through iterating through the list. Deep copies of the data can   #
#  also be made using data from another list.                                #
#                                                                            #
############################################################################*/



#include "list.h"
#include <stdlib.h>

using namespace std;


// List node constructors provided by Prof. Campbell

list_node::list_node() : prev(NULL), next(NULL) {}

list_node::list_node(list_node *p, list_node *n) : prev(p), next(n) {}

list_node::list_node(list_node *p, const list_element & d, list_node * n):
  prev(p), data(d), next(n) {}

// List constructors/destructor/assignment also provided.  
// No need to change these.

list::list() 
{
  _init();
}

list::~list()
{
  _destroy();
}

list::list(const list & orig)
{
  _copy(orig);
}

list & list::operator=(const list & rhs)
{
  if (this != &rhs) {
    _destroy();
    _copy(rhs);
  }
  return *this;
}

void list::_init()
/*#############################################################################
  Creates an empty list with a front and rear sentinel that point to each
  other. The pointers on each end of the list are set to NULL, and the pointer
  _current is set to point to the rear sentinel.
  ###########################################################################*/
{
    _size = 0;              // initializes size of list as 0
    _front = new list_node;
    _front -> prev = NULL;  // sets front sentinel
    _rear = new list_node;
    _rear -> next = NULL;   // sets rear sentinel
    _front -> next = _rear; // initializes the sentinels to point to each other
    _rear -> prev = _front;
    _current = _rear;       // points _current to the rear sentinel
    _current_index = 0;
}

void list::_copy(const list & orig)
/*#############################################################################
  Creates a new empty list. Function iterates through every element of the 
  original list and copies the elements from the original list into the new
  list.
  ###########################################################################*/
{
    _init(); // initializes a new list.
    
    // iterates through an existing list and copies each element of data 
    // until the size of the original list has been reached.
    for (size_t i = 0; i < orig.size(); i++)
        add(orig.get(i), i);
}

void list::_destroy()
/*#############################################################################
  This function deletes all nodes until only the sentinels remain by tracking 
  the number returned by size(). 

  ###########################################################################*/
{

    // loop serves to repeat the removal at index 0 until the size is 0.
    while (size() != 0) {
        remove_at(0);
    }
}

void list::add(const list_element & item, int index)
/*#############################################################################
  Creates a new node and checks whether the list is empty or if the item is to
  be placed at the end of the list. Otherwise, standard code for inserting a
  node runs. 

  ###########################################################################*/
{
    list_node * new_node = new list_node();
    new_node -> data = item;
    
    // checks to see if the data is to be placed at the end of the list or if
    // the list is empty, then connects the new node to the rear sentinel.
    if ((size_t) index == size()) {
        _set_current_index(index);
        new_node -> next = _rear;
        new_node -> prev = _rear -> prev;
        _rear -> prev -> next = new_node;
        _rear -> prev = new_node;
        _current = new_node;
    }
    
    // this part of the function executes for all other cases in which the 
    // data is to be placed inside the list.
    else {
        _set_current_index(index);
        new_node -> next = _current;
        new_node -> prev = _current -> prev;
        _current -> prev -> next = new_node;
        _current -> prev = new_node; 
        _current = new_node;
    }
    _size++;   // adds 1 to size regardless of action taken
}

void list::remove_at(int index)
/*#############################################################################
  Removes a node at a specific index.  

  ###########################################################################*/
{
    _set_current_index(index);
    list_node * p = _current;
    _current -> next -> prev = _current -> prev;   // unlinks the desired node
    _current -> prev -> next = _current -> next;
    _current = _current -> next;   // moves current so it does not get deleted
    delete p;    // deletes the node that p points to
    _size--;     // decrements size
}

void list::remove(const list_element & item)
/*#############################################################################
  Iterates through the linked list and searches for the first occurance of a 
  specific item and then deletes it.

  ###########################################################################*/
{
    _set_current_index(0); // starts from the beginning of the list

    // loop breaks once the data has been found or if the pointer _current has
    // reached the end of the list.
    while (_current -> data != item && _current -> next != NULL) {
        _current = _current -> next;
        _current_index++;
    }
    
    // calls remove_at function at the index at which the data was found.
    if (_current -> data == item) {
        remove_at(_current_index);
    }
}

size_t list::find(const list_element & item) const
/*#############################################################################
  Finds the first occurance in the linked list of a specific item, and if that
  item cannot be found, the size of the list is returned.

  ###########################################################################*/
{
    _set_current_index(0);

    // iterates through the entire list
    while (_current != _rear) {
        // onces the data has been found, the function returns the index.
        if (_current -> data == item) {
            return _current_index;
        }
        _current = _current -> next;
        _current_index++;
    }
    return size();  // if the function does not find the item in the list, 
                    // the size of the list is returned.
}

list_element list::get(int index) const
/*#############################################################################
  Retrieves the data in a node at a specific index and returns it.

  ###########################################################################*/
{
    _set_current_index(index);
    return _current -> data;  // same type as list_element
}

size_t list::size() const
// Returns the size of the linked list.
{
    return _size;
}

void list::output(std::ostream & ostr) const
/*#############################################################################
  Iterates through the linked list and prints out every element as long as the
  list is not empty.

  ###########################################################################*/
{
    // starts at the beginning of the list.
    _set_current_index(0);
    list_node * p = _current;
    ostr << '<';
    
    // checks to see if the size is not 0, then begins a loop through the
    // entire list until the second to last item of data has been printed.
    if (size() != 0) {
        while (p != _rear -> prev) {
            ostr << p -> data << ", ";
            p = p -> next;
        }
        ostr << p -> data; // prints the last item of data without a comma.
    }
    ostr << '>';
}

void list::_set_current_index(size_t index) const
/*#############################################################################
  Determines which pointer is the closest to the desired index and sets 
  _current to that pointer. _current is then moved from that position to the
  desired index.

  ###########################################################################*/
{
    // finds the distance from each pointer to the desired index
    size_t front_distance = index;
    size_t from_curr = abs((int) _current_index - (int) index);
    size_t rear_distance = (_size) - index;

    // sets _current to point to the front of the list if the front pointer is
    // closer to the desired index.
    if (front_distance < from_curr) {
        _current = _front -> next;
        _current_index = 0;
    }

    // sets _current to point to the rear sentinel of the list if the rear
    // pointer is closer to the desired index.
    else if (rear_distance < from_curr) {
        _current = _rear;
        _current_index = size();
    }

    // NOTE: _current does not change if it is closest to the desired index.

    // begins a loop until the current index is equal to the desired index by
    // accessing the 'next' pointer or 'prev' pointer of each node.
    while (_current_index != index) {
        if (_current_index < index) {
            _current = _current -> next;
            _current_index++;
        }
        else {
            _current = _current -> prev;
            _current_index--;
        }
    }
}
