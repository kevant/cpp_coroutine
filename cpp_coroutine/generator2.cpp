#include <iostream>
#include <coroutine>
#include <vector>

template<typename T>
struct Generator
{
    struct promise_type
    {
        T value;

        Generator get_return_object()
        {
            return Generator(std::coroutine_handle<promise_type>::from_promise(*this));
        }
        // don't suspend for initial_suspend, so the first yield value will be return
        // can consider suspending initial_suspend if the calculation is expensive
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() {}
        std::suspend_always yield_value(T v)
        {
            value = v;
            return {};
        }
        void return_void() { }
    };

    Generator(std::coroutine_handle<promise_type> h)
        : _handle(h)
    {
    }

    ~Generator()
    {
        if(_handle)
            _handle.destroy();
    }

    /*
    * A way to check if coroutine is finished.
    * 
    * Invoke the coroutine handle and store the result.
    * This ensures that the first value will be calculated, as initial_suspend is suspended
    * Also ensures that done() can resolve properly; i.e. only once
    * 
    * done() returns true if coroutine suspends at final suspenstion point; i.e final_suspend(),
    * done() returns false at all other suspension point
    * 
    * When a coroutine reaches its last co_yield/co_await, it will suspend, but handle.done() returns false
    * Upon the next resume(), it will complete the corooutine and reach final_suspend.
    * if final_suspend is suspended, handle.done() will return true
    */
    explicit operator bool()
    {
        fill();
        return !_handle.done();
    }

    T operator()()
    {
        fill();
        _filled = false;
        return std::move(_handle.promise().value);
    }

private:
    std::coroutine_handle<promise_type> _handle;
    bool _filled = false;

    void fill()
    {
        if (!_filled)
        {
            _handle();
            _filled = true;
        }
    }
};


Generator<int> interweaved_coroutine(const std::vector<int>& a, const std::vector<int>& b)
{
    auto generator_lambda = [](const std::vector<int>& v) -> Generator<int>
        {
            for (const auto& i : v)
            {
                co_yield i;
            }
        };

    auto generatorA = generator_lambda(a);
    auto generatorB = generator_lambda(b);

    while (generatorA || generatorB)
    {
        if (generatorA)
        {
            co_yield generatorA();
        }
        if (generatorB)
        {
            co_yield generatorB();
        }
    }
}

int main()
{
    std::vector<int> arrA{ 1, 2, 3, 4, 5 };
    std::vector<int> arrB{ 6, 7, 8 };
    auto generator = interweaved_coroutine(arrA, arrB);
    while(generator)
    {
        std::cout << "generator value: " << generator() << '\n';
    }
    std::cout << "====================================\n";
}