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
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception(){}
        std::suspend_always yield_value(T v)
        {
            value = v;
            return {};
        }
        void return_void() { }
    };

    std::coroutine_handle<promise_type> handle;

    T operator()() 
    {
        handle();
        return handle.promise().value;
    }
};


Generator<int> MyGenerator()
{
    int i = 0;
    while(true)
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
        std::cout << "generator value: " << gen() << '\n';
    }
    std::cout << "====================================\n";

    auto fib_gen = FibGenerator();
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "FibGenerator value: " << fib_gen() << '\n';
    }
    std::cout << "====================================\n";
}