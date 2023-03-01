// please put your main and code in this file. All contained.
// Use the provided mk script to compile your code. You are welcome to change the mk script if you'd like
// but know that is how we will be batch compiling code using mk on the back end so your code will need to be able
// to run when that bash script is ran.
// Most code can be grabbed from the .html file in the root.

#include <stdio.h>

typedef unsigned int u32;

char *ctable = "0123456789ABCDEF";
int  BASE = 10; 

int prints(char *s){
	while(*s != NULL){
		putchar(*s);
		s++;
	}
}

int rpu(u32 x)
{  
    char c;
    if (x){
       c = ctable[x % BASE];
       rpu(x / BASE);
       putchar(c);
    }
}

int printu(u32 x)
{
   (x==0)? putchar('0') : rpu(x);
   putchar(' ');
}

int printHexHelper(u32 x){
   char c;
   if (x){
   	c = ctable[x % 16];
   	printHexHelper(x / 16);
   	putchar(c);
   }
}

int printx(u32 x){
   if (x==0){
   	putchar('0');
   	putchar('x');
   	putchar('0');
   }
   else{
   	putchar('0');
   	putchar('x');
   	printHexHelper(x);
   }
}

int printd(int x){
   if (x<0){
   	putchar('-');
   	x*=-1;
   	printu(x);
   }
   else{
   	printu(x);
   }
}

myprintf(char *fmt, ...){

	char *cp = fmt;
	int *ip = (int *)&fmt+1;

	while(*cp != NULL){
		//spit out each character that's not %
		while(*cp != '%'){
			if(*cp == '\n'){
				putchar(*cp);
				putchar('\r');
			}
			else{		
				putchar(*cp);
			}
			cp++;
		}
		
		//cp now points to '%', so increment by one to get argument
		cp++;
		
		if (*cp == 'c'){ //char
			putchar(*ip);
		}
		else if (*cp == 's'){ //string
			prints(*ip);
		}
		else if (*cp == 'u'){ //unsigned int
			printu(*ip);
		}
		else if (*cp == 'x'){ //hex
			printx(*ip);
		}
		else if (*cp == 'd'){ //signed int
			printd(*ip);
		}
		
		cp++;
		//advance ip to next item in stack
		ip++;
	}
}

int main(int argc, char *argv[], char *env[]){
	myprintf("argc=%d, argv=%s, env=%s", argc, argv, env);
	myprintf("char=%c string=%s       dec=%u  hex=%x neg=%d\n", 'A', "this is a test",  100,   100,   -100);
}
