#ifndef HYPERCORE_ASYNC_H
#define HYPERCORE_ASYNC_H

#include <experimental/coroutine>

template<typename T> struct Async {
  std::shared_ptr<T> value;

  // Created
  Async(std::shared_ptr<T> p) : value(p) {}

  // Copied
  Async(const Async &s) : value(s.value) {}

  ~Async() {}

  // Ask for a return value
  T get() {
    return *value;
  }

  //
  // An async class must describe the promise type with a `promise_type`
  // struct. This struct contains the life-cycle methods for the promise.
  //
  struct promise_type {
    std::shared_ptr<T> ptr;

    // Promise Constructed
    promise_type() : ptr(std::make_shared<T>()) {}

    // Promise Destructed
    ~promise_type() {}

    // Value to return
    auto get_return_object() {
      return ptr;
    }

    // Coro start
    auto initial_suspend() {
      return std::experimental::suspend_never {};
    }

    // Coro return value 
    auto return_value(T v) {
      *ptr = v;
      return std::experimental::suspend_never {};
    }

    // Coro ended
    auto final_suspend() {
      return std::experimental::suspend_never {};
    }

    void unhandled_exception() {
      std::exit(1);
    }
  };
};

#endif
