#include <iostream>
#include <coroutine>

std::coroutine_handle<> s_handle;
bool s_first = true;

struct Awaiter
{
    constexpr bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> handle)
    {
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
    std::cout << "[     main] Invoke coroutine handle\n";
    s_handle();
    std::cout << "[     main] Invoke coroutine handle\n";
    s_handle();

    std::cout << "[     main] End of main()\n";
}