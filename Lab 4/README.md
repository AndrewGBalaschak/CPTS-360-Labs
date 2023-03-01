# cs360-lab4-template

Follow KC's write up of the assignment here: https://eecs.wsu.edu/~cs360/LAB4.html

Place your source code in the src/client and src/server directories. It is preferred that you use the existing client.c and server.c format to place your code, however, you may modify it as long as the `mk` sh script compiles your code (which of course you may modify such that it does compile your code.)

Please keep the client and server in separate folders.

## FAQ
### Why am I getting a bind or connection error?
It is difficult to diagnose these kind of problems in the starter code, as specific error codes are not presented. Generally, however, you may run `man` on the offending function call to see if it reports an error code to `errno`.

`errno` is an integer value that is often set by system function calls to report errors. You can use it in your code as follows:
```c
#include <errno.h> 	// include the library header
...
printf("errno=%d\n", errno);	// print errno after function fails
```
You may also get a human-readable error number via a `string.h` function call:
```c
printf("Error: %s\n", strerror(errno));	// alternative to print errno as a string
```
Note you will have to consult the `man` page for the true error corresponding with the provided `errno` value. For example, if the `connect()` function is failing, you may run `man 2 connect` to check the error value and reason.

### How do I run server and client?
Run server first, with sudo permissions. Then run client with sudo permissions. If done the opposite way the socket will never connect. 

### Do I need to run server and client on two different computers?
No-- you may run them on the same computer using two separate terminal windows and running `server` and `client` in separate directories (as is already set up in the repository).

### Why is changing the virtual root not working?
If you do `man chroot` you will notice that it says that the function requires *privileges*, that means you need to run your executable with sudo in order for it to work.

### I have a partner, do we demo together?
Demo individually.

### There's no a.out generated from `mk`
If you open the `mk` file, youll see the -o flag was used. This renamed the executable to either `client` or `server`, they're the same way as a.out so `./server`

## To Turn In:
Push your files to GitHub Classroom (see [here](https://eecs.wsu.edu/~cs360/ta_resources/howto-linux-cmds.html) for more details if you do not know how to submit using Git/GitHub Classroom).

Note you must `git push` your code to submit it. Submissions automatically occur based on the last pushed commit whenever the assignment is due.

## Autotests
No autotest for this lab.

## Samples
As usual, samples may be found at https://eecs.wsu.edu/~cs360/samples/LAB4/. You may download them via the `wget` utility (see the [How-To Linux Command page](https://eecs.wsu.edu/~cs360/ta_resources/howto-linux-cmds.html) on the course webpage if you are unfamiliar).

Note `client` and `server` are executables and may be run after using `chmod +x`, whereas `client.c` and `server.c` are starter code.

