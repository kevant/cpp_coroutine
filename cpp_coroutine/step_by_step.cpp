#include <iostream>
#include <coroutine>

struct Coro
{
    struct promise_type
    {
        int value = 0;
        Coro get_return_object() 
        {
            return { .handle = std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() {}
        void return_void() {}
        std::suspend_always yield_value(int v)
        {
            value = v;
            return {};
        }
    };

    std::coroutine_handle<promise_type> handle;
};

Coro MyCoroutine()
{
    int i = 0;
    while (true)
    {
        std::cout << "[coroutine] yield " << i << "\n";
        co_yield i++;
    }
}

int main()
{
    std::cout << "[     main] Invoke MyCoroutine()\n";
    Coro coro = MyCoroutine();
    std::coroutine_handle<Coro::promise_type> handle = coro.handle;
    Coro::promise_type& promise = handle.promise();
    std::cout << "====================================\n";

    for (int i = 0; i < 3; ++i)
    {
        handle();
        std::cout << "[     main] promise.value: " << promise.value << '\n';
    }

    handle.destroy();
    std::cout << "====================================\n";
    std::cout << "[     main] End of main()\n";
}