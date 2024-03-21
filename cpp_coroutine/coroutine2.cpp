/*
* Use std::coroutine_handle<> and Awaiter to continue coroutine
*
* co_await stores the state of the function into the heap,
* and creates a callable object, std::coroutine_handle<>
* Invoking std::coroutine_handle<> will continue the execution of the function, from its point of suspension
*
* P.S. As state of function is stored into heap, C++ coroutine is also considered to have stackless property
*/
#include <iostream>
#include <coroutine>

/*
* A static variable is used for now,
* so it can be easily assigned and invoked from different scope
*/
std::coroutine_handle<> s_handle;
bool s_first = true;

/*
* Custom Awaiter is implemented to define specific behaviors at coroutine's suspension point;
* i.e. store the coroutine handle to s_handle, so it can be invoked in main() function
*/
struct Awaiter
{
    constexpr bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> handle)
    {
        /*
        * coroutine handle does not change across invocations, so it only needs to be assigned once
        * for now, use naive way of checking against a bool to illustrate this point
        */
        if (s_first)
        {
            s_handle = handle;
            s_first = false;
        }
    }
    constexpr void await_resume() const noexcept {}
};

struct Coro
{
    struct promise_type
    {
        Coro get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void unhandled_exception() {}
        void return_void() {}
    };
};

/*
* When the line "co_await Awaiter();" runs, the compiler creates a coroutine handle 
* and passes it as agrument to the method "Awaiter::await_suspend(std::coroutine_handle<>){...}"
*/
Coro MyCoroutine()
{
    std::cout << "[coroutine] Instruction 1\n";
    co_await Awaiter();
    std::cout << "[coroutine] Instruction 2\n";
    co_await Awaiter();
    std::cout << "[coroutine] Instruction 3\n";
}

/*
* Control is passed between main() and MyCoroutine()
* When the coroutine handle is invoked, it continues from the coroutine's suspension point.
*/
int main()
{
    std::cout << "[     main] Invoke MyCoroutine()\n";
    MyCoroutine();
    std::cout << "[     main] Invoke coroutine handle\n";
    s_handle();
    std::cout << "[     main] Invoke coroutine handle\n";
    s_handle();

    std::cout << "[     main] End of main()\n";
}