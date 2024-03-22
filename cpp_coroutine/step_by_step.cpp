#include <iostream>
#include <coroutine>

struct Coro
{
    struct promise_type
    {
        Coro get_return_object() 
        {
            return { .handle = std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void unhandled_exception() {}
        void return_void() {}
    };

    std::coroutine_handle<promise_type> handle;
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
    Coro coro = MyCoroutine();
    std::coroutine_handle<> handle = coro.handle;
    std::cout << "[     main] Invoke coroutine handle\n";
    handle();
    std::cout << "[     main] Invoke coroutine handle\n";
    handle();

    std::cout << "[     main] End of main()\n";
}