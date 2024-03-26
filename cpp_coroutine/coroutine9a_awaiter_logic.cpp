#include <iostream>
#include <coroutine>

struct Awaiter
{
    bool await_ready() const noexcept
    {
        return false;
    }
    bool await_suspend(std::coroutine_handle<> handle) const noexcept
    {
        return true;
    }
    void await_resume() const noexcept{}
};

// this is an pseudo-code of what happens from the compiler's point view
void awaiter_logic()
{
    auto&& awaiter = Awaiter(); // or other Awaiter object, like std::always
    if (!awaiter.await_ready())
    {
        if (awaiter.await_suspend(std::coroutine_handle<>{}))
        {
            return; // return control back to caller
        }
    }
    awaiter.await_resume(); // start from top again, checking for await_ready()
};