## LAB3

### QUESTIONS

#### 5. What advantages can you identify in these new variants of the given programs?

- `read()` :
	1. Is a low level and unbuffered read.
		- Ex: `read()` blocks, but if you ask for 4kB it may return after reading just 1kB, even if the file has not ended.
		- `read()` will drop to the kernel for each call.
	2. Makes a direct system call on UNIX (to kernel) and that performs the IO operation.
	3. It's not formatted IO: we have a non formatted byte stream.

- `fread()` :
	1. Is a function provided in standard C library.
	2. Provides buffered reads, using an internal buffer (Linux buffer cache)
	3. It's formatted IO (with the "%.." parameter)
	4. Standard IO library functions are optimized for system calls, rather than the application making system calls.
	5. `fread()` blocks until the number of bytes you asked for has been read, or the file ends, or an error occurs.
	6. NOTE: mainly used for binary file data where struct data are stored.

##### Main Differences using system calls vs standard C library IO

	1. The main difference between these two is the number of system calls in your application. 
	2. The `read() / write()` level APIs do not do buffering - so if you read() 1 byte at a time you will have a huge performance penalty compared to doing the same thing with `fread() / fwrite()`. `fread() / write()` will pull a block and dole it out as you ask for it. `read()/ write()` will drop to the kernel for each call.
