/*
Why do we need to define Promise?

Promise defines coroutine's behavior; e.g.
- Creation of return object; get_return_object()
- Suspension (or not) at beginning of a coroutine; initial_suspend()
- Suspension at the end of a coroutine; final_suspend()
- Exception Handling; unhandled_exception()
*/

#include <iostream>
#include <coroutine>

struct Coro
{
    struct promise_type
    {
        Coro get_return_object() { return {}; }
        // change suspend_never to suspend_always and see what happens
        std::suspend_never initial_suspend() { return {}; }

        std::suspend_never final_suspend() noexcept { return {}; }
        void unhandled_exception() {}
        void return_void() {}
    };
};

/*
* Below is a simplified compiler's view of the coroutine
*/
Coro MyCoroutine()
{
    
    Coro::promise_type p{};
    Coro coro = p.get_return_object();

    try
    {
        co_await p.initial_suspend();
        co_await std::suspend_never();
        std::cout << "some code\n";
    }
    catch (...)
    {
        p.unhandled_exception();
    }

    co_await p.final_suspend();
}

int main()
{
    MyCoroutine();
}