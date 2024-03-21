/*
* Instead of storing std::coroutine_handle<> in static scope, store the handle in Coroutine Return Object.
* This allow std::coroutine_handle<> to be retrived within the scope where the coroutine is invoked
* 
* Since user-defined Awaiter is no longer needed to assign std::coroutine_handle<>, it is removed... for now
*/
#include <iostream>
#include <coroutine>

struct Coro
{
    struct promise_type
    {
        /*
        * the return value of get_return_object() is the same as the return value of the coroutine; i.e. Coro
        * get_return_object() is invoked right after coroutine function is invoked
        *   [     main] Invoke MyCoroutine()
        *   [  promise] get_return_object()
        *   ...
        */
        Coro get_return_object()
        {
            std::cout << "[  promise] get_return_object()\n";
            std::cout << "[  promise] Assign std::coroutine_handle<promise_type> to Coro::handle \n";

            // There's a static method to get coroutine handle from promise_type
            // The following uses C++20 designated initializer syntax
            return { .handle = std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_never initial_suspend()
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
    };

    std::coroutine_handle<promise_type> handle;
};

Coro MyCoroutine()
{
    std::cout << "[coroutine] Instruction 1\n";
    co_await std::suspend_always();
    std::cout << "[coroutine] Instruction 2\n";
    co_await std::suspend_always();
    std::cout << "[coroutine] Instruction 3\n";
}

int main()
{
    std::cout << "[     main] Invoke MyCoroutine()\n";
    Coro coro = MyCoroutine();
    std::coroutine_handle<> handle = coro.handle;
    std::cout << "[     main] Invoke coroutine handle 1\n";
    handle();
    std::cout << "[     main] Invoke coroutine handle 2\n";
    handle();

    std::cout << "[     main] End of main()\n";
}