#include <iostream>
#include <coroutine>

struct Coro
{
    struct promise_type
    {
        int value = 0;

        ~promise_type()
        {
            std::cout << "[  promise] destructor()\n";
        }
        Coro get_return_object()
        {
            std::cout << "[  promise] get_return_object()\n";
            return { .handle = std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend()
        {
            std::cout << "[  promise] initial_suspend()\n";
            return {};
        }
        std::suspend_always final_suspend() noexcept
        {
            std::cout << "[  promise] final_suspend()\n";
            return {};
        }
        void unhandled_exception()
        {
            std::cout << "[  promise] unhandled_exception()\n";
        }

        /*
        * Similar to yield_value, but returns void, just like return_void()
        * coroutine cannot have both return_void() and return_value(), only either 1
        */
        //void return_void(){}
        void return_value(int v)
        {
            std::cout << "[  promise] return_value(). Argument received: " << v << "\n";
            value = v;
        }

        std::suspend_always yield_value(int v)
        {
            std::cout << "[  promise] yield_value(). Argument received: " << v << "\n";
            value = v;
            return {};
        }
    };

    std::coroutine_handle<promise_type> handle;
};

Coro MyCoroutine()
{
    int i = 0;
    for (int i = 0; i < 3; ++i)
    {
        std::cout << "[coroutine] yield " << i << "\n";
        co_yield i;
    }

    std::cout << "[coroutine] co_return\n";
    co_return 99;
}

int main()
{
    std::cout << "[     main] Invoke MyCoroutine()\n";
    Coro coro = MyCoroutine();
    std::coroutine_handle<Coro::promise_type> handle = coro.handle;
    Coro::promise_type& promise = handle.promise();
    std::cout << "====================================\n";

    while (!handle.done())
    {
        handle();
        std::cout << "[     main] promise.value: " << promise.value << '\n';
    }

    std::cout << "====================================\n";
    std::cout << "[     main] End of main()\n";
}