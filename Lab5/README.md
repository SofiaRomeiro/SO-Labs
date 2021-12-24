### LAB 5

#### 2 - Thread behaviour
- For example, passing `10000` as an argument, we get :

        Alice depositou no total: 10000
        Bob gastou no total: 9995
        História chegou ao fim
        Consulta: saldo=1257, #movimentos=12815 `

- In this example, the variable `saldo` should have it's final value equal to `5`, instead of `1257`, since Bob has spent `9995` of the total amount of `1000`. This can happen due to the competition between threads when reading and writing the value currently stored in the variable.
- Since the processor deals with each thread by giving it a period of time to be executed and this period can end unexpectly, it's impossible to know if a reading succeeds a writing or something like that.
- So Bob can raise money that actually doesn't exist because when he requests an amount, it's previous raised amount maybe wasn't written yet in the variable's address (in the tiem that the system performs a read of the current value to see if it's possible to raise that amount).  
- The final balance is incoherent with the bank account's movements due to thread competition in accessing the variable.

#### Critical Sections
In this program we have:
- 1st Step : Access to shared memory
    Occurs in lines : `26-27`, `35-37`, `48-49`
- 2nd Step : Delimit dangerous sections
    Occurs in lines : `26-27`, `35-40`, `48-49`

#### Mutexes
- Function to destroy mutexes : `pthread_destroy_mutex(mutex)`

#### Mutex vs Rwlock
- CASE 1: Using sleep(1)
For both functions, compiling using `gcc -pthread -lpthread -o <name> <file-name>.c`, giving `10` as input the output was:
    `./rwlock 10  0,00s user 0,00s system 0% cpu 13,073 total` for rwlock version with 4 threads
    `./mutex 10  0,00s user 0,00s system 0% cpu 22,279 total` for mutex version with 4 threads
Which means that, in this case, `rwlock` was 1.7 times faster than `mutex`
- CASE 2: Using different number of threads calling `consultar_conta()`
For both functions, compiling using `gcc -pthread -lpthread -o <name> <file-name>.c`, giving `10000` as input the output was:
    `./rwlock 10000  0,00s user 0,00s system 162% cpu 0,003 total` for rwlock version with 4 threads
    `./mutex 10000  0,00s user 0,00s system 157% cpu 0,004 total` for mutex version with 4 threads
    -------------------------------------------------------
    `./rwlock 10000  0,01s user 0,00s system 170% cpu 0,004 total` for rwlock version with 8 threads
    `./mutex 10000  0,01s user 0,00s system 162% cpu 0,004 total` for mutex version with 8 threads
    -------------------------------------------------------
    `./rwlock 10000  0,01s user 0,00s system 178% cpu 0,004 total` for rwlock version with 16 threads
    `./mutex 10000  0,00s user 0,01s system 179% cpu 0,005 total` for mutex version with 16 threads
Which means that, in this case, by increasing the amount of threads, the results obtained with `rwlock` become better than the ones obtained with `mutex`

