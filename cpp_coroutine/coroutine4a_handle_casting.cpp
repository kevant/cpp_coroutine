/*
* There are implementation that cast Coroutine Return Object to std::coroutine_handle<>, 
* as Coroutine Return Object does not store many data. (Most are stored in promise_type)
*/
#include <iostream>
#include <coroutine>

struct Coro
{
    struct promise_type
    {
        Coro get_return_object()
        {
            std::cout << "[  promise] get_return_object()\n";
            std::cout << "[  promise] Assign std::coroutine_handle<promise_type> to Coro::handle \n";

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

    /*
    * User-defined conversion, from Coro to std::coroutine_handle<promise_type>
    * as template argument for promise_type is void by default, 2 different conversion is written.
    */
    operator std::coroutine_handle<promise_type>() const { return handle; }
    operator std::coroutine_handle<>() const { return handle; }
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
    std::coroutine_handle<> handle = MyCoroutine(); // can assign output from MyCoroutine() to std::coroutine_handle<>
    std::cout << "[     main] Invoke coroutine handle 1\n";
    handle();
    std::cout << "[     main] Invoke coroutine handle 2\n";
    handle();

    std::cout << "[     main] End of main()\n";
}