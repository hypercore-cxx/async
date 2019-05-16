# SYNOPSIS
Provides coroutine-based async/await and minimal promises.

# ASYNC/AWAIT
## USAGE
- Any function that returns an `Async<T>` type becomes a coroutine.
- Use `co_return` instead of `return` for coroutines.
- Only coroutines can use `co_await`, and only coroutines are awaitable.
- Awaitable functions have a `get` method to retrieve the value from the promise.

## EXAMPLE

```c++
auto answer = []() -> Async<int> {
  //
  // ...possibly do something async here,
  // this could also be a regular function.
  //
  co_return 42;
};

auto question = []() -> Async<int> {
  //
  // await the promise created by the answer function.
  //
  auto n = co_await answer();

  //
  // use co_return instead of return!
  //
  co_return n;
};

auto v = question().get(); // v == 42
```

# PROMISE
```c++
Promise<int> p;
Timeout timeout;

timeout.start([&] {
  p.resolve(42);
}, 512);

auto v = p.await(); // blocks until resolved is called.
```
