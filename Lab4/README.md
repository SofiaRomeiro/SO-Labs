## LAB 4 - THREADS

### Program Execution - Observations
- Initial Value = 0
- Value after creating pthread = 0
- Value after attribution = 2
- Value after p_thread join = 1

### The Sleep Function
The sleep() function suspends the execution of the requesting thread until the number of real-time seconds provided by the argument (seconds) has passed or a signal is given to the calling thread with the action of invoking a signal-catching function or terminating the process has elapsed.

### Why is the behaviour indetermined ?
- If we call the `sleep()` function, the thread running in parallel can be finished before the system comes alive again. So the final Value is completely impredictable. 
- By putting the `sleep()` call on `line 26`, right before a new Value attribution, can give enough time to the parallel thread to finish it's execution.
- In resume : the execution shown in the 2nd page of the PDF can be modified because of the `sleep()` function, simmulating a real behaviour executed by the system's processor. 

### Threads vs Processes
- If use use processes, we can expect a constant behaviour even if we use the system call `sleep()` because there are two distinct processes instead of just a single process with multiple threads inside itself. 
- So the father process will always wait for the child process (`wait(&status)`) which is responsible for the last attribution to the Value global variable.