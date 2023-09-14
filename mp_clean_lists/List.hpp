/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */


template <class T>
List<T>::List() { 
  // @TODO: graded in MP3.1
    head_ = NULL;
    tail_ = NULL;
    length_ = 0;
}


/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  // @TODO: graded in MP3.1
  return List<T>::ListIterator(head_);
}


/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {
  // @TODO: graded in MP3.1
  if (tail_ != NULL) {
    return List<T>::ListIterator(tail_->next);
  }
  return NULL;
}


/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy() {
  /// @todo Graded in MP3.1
  ListNode * temp;
  while (head_ != NULL) {
    temp = head_;
    head_ = head_ -> next;
    delete temp;
  }
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(const T & ndata) {
  /// @todo Graded in MP3.1
  ListNode * newNode = new ListNode(ndata);
  if (head_ == NULL) {
    head_ = newNode;
    tail_ = head_;
  } else {
    newNode->next = head_;
    head_->prev = newNode;
    head_ = newNode;
    if (length_ == 1) {
      tail_->prev = head_;
    }
  }
  length_++;
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) {
  /// @todo Graded in MP3.1
  ListNode * newNode = new ListNode(ndata);
  if (head_ == NULL) {
    head_ = newNode;
    tail_ = head_;
  } else {
    tail_->next = newNode;
    newNode->prev = tail_;
    tail_ = newNode;
    if (length_ == 1) {
      head_->next = tail_;
    }
  }
  length_++;
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * This function is also called by the public split() function located in
 * List-given.hpp
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
  /// @todo Graded in MP3.1
  if (start == NULL) {
    return start;
  }
  ListNode* curr = start;

  for (int i = 0; i < splitPoint || curr == NULL; i++) {
    curr = curr->next;
  }

  if (curr == head_) {
    return curr;
  }

  if (curr != NULL) {
      curr->prev->next = NULL;
      curr->prev = NULL;
  }

  return curr;
}

/**
  * Modifies List using the rules for a TripleRotate.
  *
  * This function will to a wrapped rotation to the left on every three 
  * elements in the list starting for the first three elements. If the 
  * end of the list has a set of 1 or 2 elements, no rotation all be done 
  * on the last 1 or 2 elements.
  * 
  * You may NOT allocate ANY new ListNodes!
  */
template <typename T>
void List<T>::tripleRotate() {
  // @todo Graded in MP3.1
  if (head_ == NULL || length_ < 3) {
    return;
  }
  ListNode* a = head_;
  ListNode* b = a->next;
  ListNode* c = b->next;
  while (a != NULL) {
    if (a == head_) {
      b->prev = a->prev;
      head_ = b;
    } else {
      b->prev = a->prev;
      a->prev->next = b;
    }
    if (c == tail_) {
      a->next = c->next;
      tail_ = a;
    } else {
      a->next = c->next;
      c->next->prev = a;
    }
    c->next = a;
    a->prev = c;
    a = a->next;
    if (a == NULL) {
      return;
    }
    b = a->next;
    if (b == NULL) {
      return;
    }
    c = b->next;
    if (c == NULL) {
      return;
    }
  }
}


/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
  /// @todo Graded in MP3.2
  if (startPoint == NULL || endPoint == NULL || startPoint == endPoint) {
    return;
  }
  bool head = false;
  bool tail = false;
  ListNode* prevsp = NULL;
  ListNode* nextep = NULL;
  if (startPoint == head_) {
    head = true;
  }
  if (endPoint == tail_) {
    tail = true;
  }
  if (!head) {
    prevsp = startPoint->prev;
  }
  if (!tail) {
    nextep = endPoint->next;
  }
  ListNode* curr = startPoint;
  ListNode* temp = NULL;
  while (curr != NULL && curr != prevsp) {
    if (curr == startPoint) {
      curr->prev = curr->next;
      curr->next = nextep;
      if (nextep != NULL) {
        nextep->prev = curr;
      }
    } else if (curr == endPoint) {
      curr->next = curr->prev;
      curr->prev = prevsp;
      if (prevsp != NULL) {
        prevsp->next = curr;
      }
    } else {
      temp = curr->next;
      curr->next = curr->prev;
      curr->prev = temp;
    }
    curr = curr->prev;
  }
  ListNode* temp1 = startPoint;
  startPoint = endPoint;
  endPoint = temp1;
  if (head_ == endPoint) {
    head_ = startPoint;
  }
  if (tail_ == startPoint) {
    tail_ = endPoint;
  }

}

/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n) {
  /// @todo Graded in MP3.2
  if (head_ == NULL || n == 1) {
    return;
  }
  if (n > length_) {
    reverse();
    return;
  }
  ListNode* rev = head_;
  ListNode* revnext = head_;
  for (int i = 1; i < n; ++i) {
    if (revnext == NULL) {
      break;
    }
    revnext = revnext->next;
  }
  while (rev != NULL) {
    reverse(rev, revnext);
    for (int i = 0; i < n; ++i) {
      if (revnext == NULL) {
        for (int j = 0; j < n-i; ++j) {
          rev = rev->next;
        }
        reverse(rev, tail_);
        return;
      }
      rev = rev->next;
      revnext = revnext->next;
    }
  }
}


/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
  /// @todo Graded in MP3.2
  ListNode* start = NULL;
  if (first->data < second->data) {
    start = first;
    first = first->next;
  } else {
    start = second;
    second = second->next;
  }
  ListNode* merger = start;
  while (start != NULL) {
    if (first == NULL) {
      while (second != NULL) {
        start->next = second;
        second = second->next;
        start = start->next;
      }
    } else if (second == NULL) {
      while (first != NULL) {
        start->next = first;
        first = first->next;
        start = start->next;
      }
    } else if (first != NULL && second != NULL) {
      if (first->data < second->data) {
        start->next = first;
        first = first->next;
      } else {
        start->next = second;
        second = second->next;
      }
    }
    tail_ = start;
    start = start->next;
  }
  head_ = merger;
  return merger;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * Called by the public sort function in List-given.hpp
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
  /// @todo Graded in MP3.2
  if (start == NULL || start->next == NULL || chainLength == 1) {
    return start;
  }
  ListNode* merger = split(start, (int)(chainLength/2));

  ListNode* one = mergesort(start, (int)(chainLength/2));
  ListNode* two = NULL;
  if (chainLength % 2 == 0) {
    two = mergesort(merger, (int)(chainLength/2));
  }
  else {
    two = mergesort(merger, (int)(chainLength/2) + 1);
  }
  
  return merge(one, two);
}
