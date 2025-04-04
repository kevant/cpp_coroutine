# C++ Coroutine
Notess on C++ coroutine. This is written as C++ coroutine can be quite difficult to understand.<br/>
This is my attempt to make it easier by breaking C++ coroutine down into smaller chunks, for future reference.<br/>

# Analysis
## Pros 
- Very useful if there's multiple or other computing resources, e.g. GPU, TPU, async I/O etc
- Potentially can gain a lot of performance, when implemented properly, e.g. Multi-threaded task scheduler
- Unlike callback functions, it can share/read local variables.

## Cons
- More boilerplate code required, compared to something like Unity's coroutine
- Code does not look intuitive
- Due to the way it is designed in C++, a very thorough understanding of coroutine is needed, which is a very difficult topic by itself

At this point of writing, I am more inclined to the idea that function callbacks are easier.<br/>
This is because coroutine can crash easily if not written properly; e.g. 
- Coroutine allocates object to heap (as it is stackless), hence it is more prone to runtime crashes
- If there's no callable object created to the heap, it will crash
- Callable object will not be created if there's nothing to resume execution from


## Aside (for myself)
- coroutine handle should be destroyed if coroutine is not completed
- If coroutine is completed or fall through till the end, DO NOT destory(). it will crash. This is crucial for co_return and co_yield stuff
- The nuances of await_ready() and await_suspend() can be tricky. Without proper understanding, the coroutine can suspend in weird points, which can be hard to reason
- Depending on suspension points, handle.resume() will invoke await_resume() instead. >.>

## TODO
Single-threaded Task Scheduler example
Line Reader example