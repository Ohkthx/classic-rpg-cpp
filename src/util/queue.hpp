#ifndef _QUEUE_HPP
#define _QUEUE_HPP

#include <deque>
#include <iostream>
#include <vector>

template <typename T> class FixedQueue {
private:
  std::deque<T> deque;
  std::size_t max_size;

public:
  // Constructor initializes the maximum size of the queue.
  explicit FixedQueue(std::size_t max_size) : max_size(max_size) {}

  // Adds an element to the queue. If the queue is full, it removes the oldest
  // element.
  void push(const T &value) {
    if (deque.size() == max_size) {
      deque.pop_front(); // Remove the oldest element
    }
    deque.push_back(value); // Add the new element
  }

  // Removes and returns the oldest element from the queue.
  T pop() {
    if (deque.empty()) {
      throw std::runtime_error("Attempting to pop from an empty queue.");
    }
    T front = deque.front();
    deque.pop_front();
    return front;
  }

  // Returns whether the queue is empty.
  bool isEmpty() const { return deque.empty(); }

  // Returns the current size of the queue.
  size_t size() const { return deque.size(); }

  // Optional: A utility function to print the contents of the queue for
  // debugging.
  void print() const {
    for (const auto &item : deque) {
      std::cout << item << " ";
    }
    std::cout << "\n";
  }

  // Function to return the data as a std::vector
  std::vector<T> toVector() const {
    std::vector<T> vec;
    vec.reserve(deque.size());
    for (const auto &item : deque) {
      vec.push_back(item);
    }
    return vec;
  }
};

#endif
