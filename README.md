# C++ Coroutine
Notess on C++ coroutine. This is written as C++ coroutine can be quite difficult to understand.<br/>
This is my attempt to make it easier by breaking C++ coroutine down into smaller chunks, for future reference.<br/>


## Aside (for myself)
- coroutine handle should be destroyed if coroutine is not completed
- If coroutine is completed or fall through till the end, DO NOT destory(). it will crash. This is crucial for co_return and co_yield stuff
- The nuances of await_ready() and await_suspend() can be tricky. Without proper understanding, the coroutine can suspend in weird points, which can be hard to reason
- Depending on suspension points, handle.resume() will invoke await_resume() instead. >.>

## TODO
Single-threaded Task Scheduler example
Line Reader example