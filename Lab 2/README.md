# cpts360-lab-2-template

Follow KC's write up of the assignment here: https://eecs.wsu.edu/~cs360/LAB2.html

Place your source code in the src/ directory. It is preferred that you use the existing t.c format to place your code, however, you may modify it as long as the `mk` sh script compiles your code (which of course you may modify such that it does compile your code.)

## To Turn In:
Push your files to GitHub Classroom (see [here](https://eecs.wsu.edu/~cs360/ta_resources/howto-linux-cmds.html) for more details if you do not know how to submit using Git/GitHub Classroom).

Note you must `git push` your code to submit it. Submissions automatically occur based on the last pushed commit whenever the assignment is due.

## Autotests
Autotests are run on your code when you push to check if it is compiling or not. To be fully transparent, the following commands will be used to check your code (if you wish to manually verify prior to pushing):
```
cd src && ./mk
```
```
cd src && rm a.out && ./mk && ls
```
Note this should just compile your program. Please **do not** add a.out to this script-- as you will not pass the autotest (as your program most likely needs user input).

You may confirm if your latest commit is passing the autotests by going to your online repositoy (after performing a `git push`) and going to the "Actions" tab-- the top commit should have a green check mark next to it if it is passing.

Also note you are **only emailed if you fail the autotests, not if you pass!** Therefore it is important to check that your commit is passing on the website.

## Samples
As usual, samples may be found at https://eecs.wsu.edu/~cs360/samples/LAB2. You may download them via the `wget` utility (see the [How-To Linux Command page](https://eecs.wsu.edu/~cs360/ta_resources/howto-linux-cmds.html) on the course webpage if you are unfamiliar).

Note both `lab2.bin` and `lab2.static.bin` are executables and may be run after using `chmod +x`, whereas `lab2base.c` is starter code.

If you are having difficulty pressing "enter" in the sample solution, run the following command:
```
stty sane
```
