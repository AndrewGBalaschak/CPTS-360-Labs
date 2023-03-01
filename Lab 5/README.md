# cpts360-lab5-template

Follow KC's write up of the assignment here: https://eecs.wsu.edu/~cs360/showblock.html

The command `./mk` must compile your code.

If your computer cannot find the ext2 library, you may need to import them with:
```
sudo apt-get install e2fslibs-dev
```

## General Tips:
- make use of the gnu documentation of ext-2, this is the best way to become familiar with the structs and file systems. Imo the best thing you can do is be able to visualize the system in memory, then you will have no problem writing the code for it. [Documentation is linked here](https://www.nongnu.org/ext2-doc/ext2.html)
- Come up with mneumonics for each variable that KC uses, he will be using them for the rest of the semester, e.g *ip is inode pointer, ino is inode number etc...

## Specific Tips:
- Being able to visualize the indirect blocks is the best way to make sure you're implementing them, we've added a picture to the repo.
- *important*: be aware that when you have something like ip = (INODE *) buf +1;, if you change buff by doing another read(fd, ip -> i_block[0], buf);, you are corrupting that ip pointer because now it is pointing at whatever you just read into the buffer. If you are getting garbage numbers like 111022310 as your output, that is most likely the culprit. 
- remember that indirect blocks contain 4 byte (the size of an integer) locations of other nodes - however the buffer is a character array where each character is 1 byte. Therefore if you are iterating through the block for indirect nodes, you need to make sure you are iterating 4 bytes at a time. 
- path2node is the search function. KCW refers to them both as if they're seperate things. Once you get search or path2node implemented, you don't need to do the other. 

## Some bash commands:
Uses these commands:
```
mkdir -p temp
sudo mount vdisk temp
``` 
To be able to explore the contents of vdisk normally. i.e `cd temp; ls -l`.
To remove the mounted file system do:
```
sudo umount temp
``` 

While you're testing your file system you really cant trust that you're not corrupting the data on it so adding:
`wget https://eecs.wsu.edu/~cs360/samples/LAB5/vdisk`
to your mk file can help make sure you're getting a valid disk every time. You can also just copy the disk every time with:
```
rm vdisk
cp copied_vdisk vdisk
```


## To Turn In:
Push your files to GitHub Classroom (see [here](https://eecs.wsu.edu/~cs360/ta_resources/howto-linux-cmds.html) for more details if you do not know how to submit using Git/GitHub Classroom).

Note you must `git push` your code to submit it. Submissions automatically occur based on the last pushed commit whenever the assignment is due.

## Autotests
There will be no auto-test for this assignment. BUT when we test your code we will be testing if double and indirect blocks are working with the command:
```./a.out /cs360/is/fun/huge```
So ensure that is working if you would like full credit. KC's example can be run to see what intended output should be. Basically it is just integer values of `int ino` gathered from the blocks.
