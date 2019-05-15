# SYNOPSIS
Async/await using coroutines!

# USAGE

```c++
auto answer = []() -> Async<int> {
  co_return 42;
};

auto promise = answer();
auto v = promise.get(); // v == 42
```

```c++
Async<string> foobar () {
  co_return "bazz";
}

auto p = foobar();
auto v = p.get(); // v == "bazz"
```
