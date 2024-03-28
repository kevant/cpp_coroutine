#include <iostream>
#include <coroutine>

template<typename T>
struct Generator
{
    struct promise_type
    {
        T value;

        Generator get_return_object()
        {
            return { .handle = std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        // don't suspend for initial_suspend, so the first yield value will be return
        // can consider suspending initial_suspend if the calculation is expensive
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() {}
        std::suspend_always yield_value(T v)
        {
            value = v;
            return {};
        }
        void return_void() { }
    };

    std::coroutine_handle<promise_type> handle;

    T get_value()
    {
        return handle.promise().value;
    }

    void resume()
    {
        // the check is performed to ensure that
        // handle will not be invoked if handle.done() == true
        if (!handle.done())
            handle.resume();
    }
};

Generator<int> MyGenerator()
{
    int i = 0;
    while (true)
        co_yield i++;
}

Generator<int> FibGenerator()
{
    int a = 0;
    int b = 1;
    while (true)
    {
        co_yield a; // yield value first

        // do calculation upon subsequence invocation
        int temp = a;
        a = b;
        b = a + temp;
    }
}

int main()
{
    auto gen = MyGenerator();
    for (int i = 0; i < 3; ++i)
    {
        std::cout << "generator value: " << gen.get_value() << '\n';
        gen.resume();
    }
    std::cout << "====================================\n";

    auto fib_gen = FibGenerator();
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "generator value: " << fib_gen.get_value() << '\n';
        fib_gen.resume();
    }
    std::cout << "====================================\n";
}