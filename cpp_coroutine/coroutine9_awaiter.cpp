/*
Awaiter, or Awaitable controls a specific suspension point behavior
std::suspend_always and std::suspend_never are Awaiter provide by Standard library

    _EXPORT_STD struct suspend_never {
        _NODISCARD constexpr bool await_ready() const noexcept {
            return true;
        }
        ...
    };

    _EXPORT_STD struct suspend_always {
        _NODISCARD constexpr bool await_ready() const noexcept {
            return false;
        }
        ...
    };


To define Awaiter, the following method must be defined:
1. bool await_ready();
2. bool/void await_suspend(std::coroutine_handle<> handle); 
    2a. await_suspend() can return void or bool
3. void await_resume();

Promise defines coroutine behavior; i.e.
- initial_suspend()
- final_suspend()
Both methods returns an Awaiter

Awaiter controls suspension point behavior; 
*/

#include <iostream>
#include <coroutine>

std::coroutine_handle<> s_handle;
bool s_first = true;

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

    /*
    * if std::suspend_never (return true), coroutine will never suspend, hence std::suspend_never
    * await_resume() will be invoked after this method call
    * First line of coroutine function will run
    *
    * if std::suspend_always (return false), coroutine will suspend, hence std::suspend_always,
    * await_suspend() will be invoked after this method call
    */
    bool await_ready() const noexcept
    {
        std::cout << "[  awaiter] await_ready(). Return " << boolToSuspendString(suspend_never) << "\n";
        return suspend_never;
    }

    /*
    * Can return void or return bool
    * return void is same as return true
    * 
    * if return false, coroutine will not suspend and continue 
    * if return true, coroutine will suspend, passing control back to caller
    */
    bool await_suspend(std::coroutine_handle<> handle) const noexcept
    {
        std::cout << "[  awaiter] await_suspend(). Return " << boolToString(await_suspend_return) << "\n";
        if (s_first)
        {
            std::cout << "[  awaiter] Assign coroutine handle\n";
            s_handle = handle;
            s_first = false;
        }
        return await_suspend_return;
    }

    /*
    * This is invoked when coroutine resumes
    */
    void await_resume() const noexcept
    {
        std::cout << "[  awaiter] await_resume()\n";
    }
};

// this is an example code of what happens from the compiler's point view
void example()
{
    // this is what happens to compiler's point view
    auto&& awaiter = Awaiter(); // or any Awaiter object like std::always
    if (!awaiter.await_ready())
    {
        if (awaiter.await_suspend(std::coroutine_handle<>{}))
        {
            return; // return control back to caller
        }
    }
    // when control passed back or coroutine is not suspended
    awaiter.await_resume();
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

            /*
            * await_ready() -> true  = std::suspend_never
            * await_ready() -> false = std::suspend_always
            */
            return Awaiter{ .suspend_never = false };
        }
        Awaiter final_suspend() noexcept
        {
            std::cout << "[  promise] final_suspend()\n";
            return Awaiter{ .suspend_never = false };
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
    co_await Awaiter{ .suspend_never = false };
    std::cout << "[coroutine] Instruction 2\n";
    co_await Awaiter{ .suspend_never = false };
}

int main()
{
    int i = 0;
    std::cout << "[     main] Invoke MyCoroutine()\n";
    MyCoroutine();
    std::cout << "====================================\n";

    while (!s_handle.done())
    {
        std::cout << "[     main] Invoke coroutine handle " << ++i << "\n";
        s_handle();
        std::cout << "[     main] End of while scope " <<  i << "===\n";
    }

    std::cout << "====================================\n";
    std::cout << "[     main] End of main()\n";
}