#include "../deps/heapwolf/cxx-tap/index.hxx"
#include "../index.hxx"

#include <iostream>

int main() {
  TAP::Test t;

  t.test("sanity", [](auto t) {
    t->ok(true, "true is true");
    t->end();
  });

  t.test("await", [] (auto t) {
    using namespace Hyper::Util;
    using namespace std::chrono;

    auto answer = []() -> Async<int> {
      //
      // for fun let's do something async in here
      // using std::async, wait, get and return.
      //
      auto a = std::async(std::launch::async, [&] {
        std::this_thread::sleep_for(seconds(2));
        return 42;
      });

      a.wait();
      auto n = a.get();

      co_return n;
    };

    auto question = [&]() -> Async<int> {
      auto a = answer();
      auto v1 = co_await a;
      auto v2 = co_await a;

      t->equal(v1, v2, "awaited values are the same every time");

      co_return v2;
    };

    auto n = question().get();
    t->equal(n, 42, "final value of async functions is correct");

    t->end();
  });

  t.test("promise", [] (auto t) {
    Hyper::Util::Promise<int> p;
    Util::Timeout timeout;

    std::vector<int> order;

    order.push_back(0);

    timeout.start([&] {
      order.push_back(2);
      t->ok(true, "promise resolved from timer thread");
      p.resolve(42);
    }, 512);

    order.push_back(1);

    auto v = p.await();

    t->equal(order[0], 0);
    t->equal(order[1], 1);
    t->equal(order[2], 2);
    t->equal(v, 42);
    t->end();
  });

  t.test("another sanity check", [](auto t) {
    using namespace std::chrono;

    auto f1 = std::async(std::launch::async, [&] {
      std::this_thread::sleep_for(seconds(2));
      return 1;
    });

    auto f2 = std::async(std::launch::async, [&] {
      std::this_thread::sleep_for(seconds(2));
      return 1;
    });

    Util::Timer t1;
    f1.wait();
    f2.wait();

    auto round = [](int number) {
      return ((number + 1000 / 2) / 1000) * 1000;
    };
 
    t->equal(2000, round(t1.ms()));

    Util::Timer t2;
    std::this_thread::sleep_for(seconds(2));
    std::this_thread::sleep_for(seconds(2));

    t->equal(4000, round(t2.ms()));
    t->end();
  });
}
