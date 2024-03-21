/*
* co_yield operator:
* Use to return value from coroutine, through promise_type object
*
* To use co_yield, promise_type need to implement yield_value(param) method
*/
#include <iostream>
#include <coroutine>

struct Coro
{
    struct promise_type
    {
        // stores return value 
        int value = 0;

        Coro get_return_object()
        {
            std::cout << "[  promise] get_return_object()\n";
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
        /*
        * When co_yield expression is used, the value/object will be passed to this method
        * stores value within promise_type, so it can be accessed from coroutine caller
        */
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
    std::cout << "[coroutine] yield 1;\n";
    co_yield 1;
    std::cout << "[coroutine] yield 2;\n";
    co_yield 2;
    std::cout << "[coroutine] Instruction 3\n";
}

int main()
{
    std::cout << "[     main] Invoke MyCoroutine()\n";
    Coro coro = MyCoroutine();
    std::coroutine_handle<Coro::promise_type> handle = coro.handle;
    Coro::promise_type& promise = handle.promise();
    std::cout << "[     main] promise.value: " << promise.value << '\n';

    std::cout << "[     main] Invoke coroutine handle 1\n";
    handle();
    std::cout << "[     main] promise.value: " << promise.value << '\n';

    std::cout << "[     main] Invoke coroutine handle 2\n";
    handle();
    std::cout << "[     main] promise.value: " << promise.value << '\n'; // this will print garbage value, as coroutine state is destroyed after it's final instruction

    std::cout << "[     main] End of main()\n";
}