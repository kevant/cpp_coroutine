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

// This highlights the relationship between handle, address, and prmise
struct Coro2
{
    struct promise_type
    {
        void return_void() {}
        void unhandled_exception() {}

        std::suspend_always initial_suspend() noexcept { return{}; }
        std::suspend_always final_suspend() noexcept { return{}; }

        // example of CoroutineHandle methods
        void examples()
        {
            // handle_from_promise from promise
            auto handle_from_promise = std::coroutine_handle<promise_type>::from_promise(*this);

            // address from handle_from_promise
            auto address = handle_from_promise.address();

            // handle_from_promise from address
            auto handle_from_address = std::coroutine_handle<promise_type>::from_address(address);

            // promise from handle_from_promise
            auto promise = handle_from_address.promise();

            //// handle_from_promise's methods
            handle_from_promise.resume(); // resumes coroutine
            //handle_from_promise.done(); // check if coroutine is completed
            handle_from_promise.destroy(); // destroy coroutine
        }
    };
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