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
As seen in the example, await_suspend() method has custom behavior; i.e. assigning coroutine_handle
*/

#include <iostream>
#include <coroutine>



struct Awaiter
{
    /*
    * if return true, coroutine will never suspend, hence std::suspend_never
    * It will not invoke await_suspend()
    *
    * if return false, coroutine will suspend, hence std::suspend_always,
    * invoking await_suspend() method
    */
    bool await_ready() const noexcept
    {
        std::cout << "[  awaiter] await_ready()\n";
        return false;
    }

    /*
    * Can return void or return bool
    * return void is same as return true
    * 
    * if return false, coroutine will not suspend and continue 
    * if return true, coroutine will suspend, passing control back to caller
    */
    bool await_suspend(std::coroutine_handle<> handle)
    {
        std::cout << "[  awaiter] await_suspend()\n";
        
        return false;
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
        Coro get_return_object() { return {}; }

        // change initial_suspend to suspend_always
        // then the coroutine will stop at initial_suspend()
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

        void unhandled_exception() {}
        void return_void() {}
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
}