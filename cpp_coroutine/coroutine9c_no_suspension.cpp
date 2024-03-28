/*
* In this example, the Awaiter will return false for await_ready (or std::suspend_always), invoking await_suspend()
* await_suspend() will return false, thus the coroutine will not suspend.
* This will also cause the coroutine function to behave similar to a normal synchronous function,
* as it will not return control back to caller until it finished
*/

#include <iostream>
#include <coroutine>

std::string boolToString(bool b)
{
    return b ? "true" : "false";
}

std::string boolToSuspendString(bool b)
{
    return b ? "suspend_never" : "suspend_always";
}

struct Awaiter
{
    bool suspend_never = false;
    bool await_suspend_return = true;

    bool await_ready() const noexcept
    {
        std::cout << "[  awaiter] await_ready(). Return " << boolToSuspendString(suspend_never) << "\n";
        return suspend_never;
    }

    // this method will not be invoked, as await ready is always true, or suspend_never
    bool await_suspend(std::coroutine_handle<> handle) const noexcept
    {
        std::cout << "[  awaiter] await_suspend(). Return " << boolToString(await_suspend_return) << "\n";
        return await_suspend_return;
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
            return Awaiter{ .suspend_never = false, .await_suspend_return = false };
        }
        Awaiter final_suspend() noexcept
        {
            std::cout << "[  promise] final_suspend()\n";
            return Awaiter{ .suspend_never = false, .await_suspend_return = false };
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
    co_await Awaiter{ .suspend_never = true, .await_suspend_return = false };
    std::cout << "[coroutine] Instruction 2\n";
    co_await Awaiter{ .suspend_never = true, .await_suspend_return = false };
}

int main()
{
    std::cout << "[     main] Invoke MyCoroutine()\n";
    Coro coro = MyCoroutine();
    std::coroutine_handle<Coro::promise_type> handle = coro.handle;
    std::cout << "====================================\n";

    // nothing from here will be printed,
    // as the coroutine is finished and handle is destroyed
    // invoking handle will cause access violation
    while (handle.address() != nullptr)
    {
        std::cout << "[     main] Invoke coroutine handle\n";
        handle();
    }

    std::cout << "====================================\n";
    std::cout << "[     main] End of main()\n";
}