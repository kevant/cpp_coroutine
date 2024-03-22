#include <iostream>
#include <coroutine>

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

Coro MyCoroutine()
{
    std::cout << "[coroutine] Instruction 1\n";

    co_await std::suspend_always();

    std::cout << "[coroutine] Instruction 2\n";
    std::cout << "[coroutine] Instruction 3\n";
}

int main()
{
    std::cout << "[     main] Invoke MyCoroutine()\n";
    MyCoroutine();

    std::cout << "[     main] End of main()\n";
}