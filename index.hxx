#ifndef HYPER_UTIL_ASYNC_H
#define HYPER_UTIL_ASYNC_H

#include <future>
#include <experimental/coroutine>

namespace Hyper {
  namespace Util {
    template<typename T> struct Async {
      struct promise_type;
      using handle_type = std::experimental::coroutine_handle<promise_type>;
      handle_type coro;

      Async(handle_type h) : coro(h) {}

      Async(const Async &) = delete;
      Async(Async &&s) : coro(s.coro) {
        s.coro = nullptr;
      }

      ~Async() {
        if (coro) {
          coro.destroy();
        }
      }

      Async &operator = (const Async &) = delete;
      Async &operator = (Async &&s) {
        coro = s.coro;
        s.coro = nullptr;
        return *this;
      }

      T get() {
        if (!this->coro.done()) {
          this->coro.resume();
        }

        auto p = coro.promise();
        return p.value;
      }

      auto operator co_await() {
        struct awaitable_type {
          handle_type coro;

          bool await_ready() {
            const auto ready = coro.done();
            return coro.done();
          }

          void await_suspend(std::experimental::coroutine_handle<> awaiting) {
            coro.resume();
            awaiting.resume();
          }

          auto await_resume() {
            const auto r = coro.promise().value;
            return r;
          }
        };

        return awaitable_type{coro};
      }


      struct promise_type {
        T value;

        promise_type() {}
        ~promise_type() {}

        auto get_return_object() {
          return Async<T>{handle_type::from_promise(*this)};
        }

        // lazy, not lazy
        auto initial_suspend() {
          // return std::experimental::suspend_never{};
          return std::experimental::suspend_always{};
        }

        auto return_value(T v) {
          value = v;
          return std::experimental::suspend_never{};
        }

        auto final_suspend() {
          return std::experimental::suspend_always{};
        }

        void unhandled_exception() {
          std::exit(1);
        }
      };
    };

    template<typename T> struct Promise {
      std::promise<T> p;
      std::shared_future<T> f;

      T await () {
        return f.get();
      }

      void resolve (T val) {
        p.set_value(val);
      }

      Promise () {
        this->f = p.get_future();
      }
    };
  } // namespace Util
} // namespace Hypercore

#endif
