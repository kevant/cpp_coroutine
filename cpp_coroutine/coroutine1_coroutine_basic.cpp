/*
* How to define a Coroutine function
    1. Use one of the 3 keywords inside the function; co_await, co_yield and/or co_return
    2. The function must return a Coroutine Return Object
	3. Coroutine Return Object have a nested promise_type struct; e.g. Coro::promise_type

* In this example, Control is passed from MyCoroutine() back to main(), through co_await
*/
#include <iostream>
#include <coroutine>

/*
* Coroutine Return Object.
* required to create coroutine function.
* It must have promise_type inner struct.
* promise_type must support certain methods (which will be further elaborated)
*/
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
* To create a coroutine function, the function needs to have
* Coroutine Return Object as its return type.
* To suspend coroutine, use co_await, co_yield or co_return,
* followed by an Awaiter. For now, a pre-defined awaiter is used, std::suspend_always()
*/
Coro MyCoroutine()
{
    std::cout << "[coroutine] Instruction 1\n";

    // suspend coroutine, returning control back to caller
    co_await std::suspend_always();

    // code after co_await will not run, until coroutine is invoked again
    std::cout << "[coroutine] Instruction 2\n";
    std::cout << "[coroutine] Instruction 3\n";
}

int main()
{
    // following code will only print 3 lines
    std::cout << "[     main] Invoke MyCoroutine()\n";
    MyCoroutine(); // only print once

    std::cout << "[     main] End of main()\n";
}