/*
* Logged all the methods to see sequence
* 
* Do take note of the return value of await_ready. Based the return (suspend_always vs suspend_never)
* it may run the code before the first co_await, which will change coroutine behavior
*/

#include <iostream>
#include <coroutine>
#include <string>

std::coroutine_handle<> s_handle;
bool s_first = true;

struct Awaiter
{
    _NODISCARD bool await_ready() const noexcept
    {
        std::cout << "[  awaiter] await_ready()\n";
        return false;
    }

    void await_suspend(std::coroutine_handle<> handle) const noexcept
    {
        std::cout << "[  awaiter] await_suspend()\n";
        if (s_first)
        {
            std::cout << "[  awaiter] Assign coroutine handle\n";
            s_handle = handle;
            s_first = false;
        }
    }
    void await_resume() const noexcept
    {
        std::cout << "[  awaiter] await_resume()\n";
    }
};

struct Coro
{
    struct promise_type
    {
        ~promise_type()
        {
            std::cout << "[  promise] ~promise_type()\n";
        }
        Coro get_return_object()
        {
            std::cout << "[  promise] get_return_object()\n";
            return {};
        }
        Awaiter initial_suspend()
        {
            std::cout << "[  promise] initial_suspend()\n";
            return Awaiter();
        }
        Awaiter final_suspend() noexcept
        {
            std::cout << "[  promise] final_suspend()\n";
            return Awaiter();
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
};

Coro MyCoroutine()
{
    std::cout << "[coroutine] Instruction 1\n";
    co_await Awaiter();
    std::cout << "[coroutine] Instruction 2\n";
    co_await Awaiter();
    std::cout << "[coroutine] Instruction 3\n";
}

int main()
{
    std::cout << "[     main] Invoke MyCoroutine()\n";
    MyCoroutine();
    std::cout << "====================================\n";

    std::cout << "[     main] Invoke coroutine handle 1\n";
    s_handle();
    std::cout << "[     main] Invoke coroutine handle 2\n";
    s_handle();

    std::cout << "====================================\n";
    std::cout << "[     main] End of main()\n";
}