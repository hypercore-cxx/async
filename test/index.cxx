#include "../deps/heapwolf/cxx-tap/index.hxx"
#include "../index.hxx"
#include <iostream>

int main() {
  TAP::Test t;

  t.test("sanity", [](auto t) {
    t->ok(true, "true is true");
    t->end();
  });

  t.test("ctz", [] (auto t) {
    auto answer = []() -> Async<int> {
      co_return 42;
    };

    auto a = answer();
    auto v = a.get();
    t->equal(v, 42);
    t->end();
  });
}
