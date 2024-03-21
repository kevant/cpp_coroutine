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

        /*
        * change from suspend_never to suspend_always,
        * so the coroutine will suspend straight away when MyCoroutine() is invoked.
        * change between suspend_never and suspend_always to see the effect
        */
        std::suspend_always initial_suspend()
        {
            std::cout << "[  promise] initial_suspend()\n";
            return {};
        }

        /*
        * change from suspend_never to suspend_always.
        * This is important, as it set make handle.done() to be true.
        * If final_suspend() returns suspend_never, done() is false,
        * and coroutine handle will be invoked again. This will cause a crash.
        */
        std::suspend_always final_suspend() noexcept
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
    while (true)
    {
        // the following code is not ideal,
        // but it is to illustrate what happens when co_return is invoked
        if (i < 3)
        {
            std::cout << "[coroutine] yield " << i << "\n";
            co_yield i++;
        }
        else
        {
            std::cout << "[coroutine] co_return\n";
            co_return;
        }
    }
}

int main()
{
    std::cout << "[     main] Invoke MyCoroutine()\n";
    Coro coro = MyCoroutine();
    std::coroutine_handle<Coro::promise_type> handle = coro.handle;
    Coro::promise_type& promise = handle.promise();
    std::cout << "====================================\n";

    while(!handle.done())
    {
        handle();
        std::cout << "[     main] promise.value: " << promise.value << '\n';
    }

    std::cout << "====================================\n";
    std::cout << "[     main] End of main()\n";
}