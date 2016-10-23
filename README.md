(1) Multithreaded Client-Server Architecture Simulation

    -Server listens to  client request and starts a service thread.At this time only, Server again starts to listen new requests  from clients.
    -Service threads are concurrent and use resouces with mutual exclusion.

(2)Fast Matrix Multiplier using pthreads on linux

First of all,my program reads dimensions of input matrices.Then it dynamically allocates memory them.
Similarly, then it allocates memory to thread pool as per user given size.Then it starts each thread with starter function as matrixMul(int ).Now the multithreading starts with  each of the entry in the matrix product will be computed by an individual thread. Then any available thread from the pool, instruct it to compute an entry of the product matrix. The thread after doing its task will return back to the pool, and will be available for further work.
Also it joins all of them just after initiating so that process doesn’t exit before completing any one of them.

- After varying input thread pool size,Initially there is  decrease in runtime,then after some interval it starts increasing.
- I have implemented printing ‘misMatch’ percent which verifies whether calculated multiplication is right or not.
- Used mutex lock/unlock at appropriate position/time.
