#include <iostream>
#include <coroutine>

struct Coro
{
    struct promise_type
    {
        int value = 0;

        Coro get_return_object()
        {
            std::cout << "[  promise] get_return_object()\n";
            return { .handle = std::coroutine_handle<promise_type>::from_promise(*this) };
        }

        /*
        * change from suspend_never to suspend_always,
        * so the coroutine will suspend straight away when MyCoroutine() is invoked.
        * change around to see the effect
        */
        std::suspend_always initial_suspend()
        {
            std::cout << "[  promise] initial_suspend()\n";
            return {};
        }
        std::suspend_never final_suspend() noexcept
        {
            std::cout << "[  promise] final_suspend()\n";
            return {};
        }
        void unhandled_exception()
        {
            std::cout << "[  promise] unhandled_exception()\n";
        }
        void return_void()
        {
            std::cout << "[  promise] return_void()\n";
        }

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

    // handle needs to be destroy() if it is not completed
    // in this case, it will never end as it has while loop inside
    handle.destroy();
    std::cout << "====================================\n";
    std::cout << "[     main] End of main()\n";
}