// Your C source code will go here.
// Feel free to replace this file with lab2base.c:
// (note this will get overwrite of the contents of this file)
// 	wget https://eecs.wsu.edu/~cs360/samples/LAB2/lab2base.c && mv lab2base.c t.c

#include <stdio.h>              // for I/O
#include <stdlib.h>             // for exit()
#include <libgen.h>             // for dirname()/basename()
#include <string.h>             // for string functions

#define true 1
#define false 0

typedef struct node{
        char  name[64];         // node's name string
        char  type;             // type = 'D' or 'F'  
   struct node *child, *sibling, *parent;
}NODE;


NODE *root, *cwd, *start;       // root->/, cwd->CWD, start->start_node
char line[128];                 // user input line
char command[16], pathname[64]; // command pathname strings

// you need these for dividing pathname into token strings
char gpath[128];                // token string holder
char *name[32];                 // token string pointers
int  n;                         // number of token strings

char dname[64], bname[64];		  // for decomposed dir_name and base_name

//              0        1     2       3        4     5        6		7
char *cmd[] = {"mkdir", "ls", "quit", "rmdir", "cd", "creat", "rm", "pwd", 0};

int findCmd(char *command)
{
   int i = 0;
   while(cmd[i]){
     if (strcmp(command, cmd[i])==0)
         return i;
     i++;
   }
   return -1;
}

int dbname(char *pathname){
	char temp[128];
	strcpy(temp, pathname);
	strcpy(dname, dirname(temp));
	strcpy(temp, pathname);
	strcpy(bname, basename(temp));
}


NODE *search_child(NODE *parent, char *name)
{
  NODE *p;
  printf("search for %s in parent DIR %s\n", name, parent->name);
  p = parent->child;
  if (p==0)
    return 0;
  while(p){
    if (strcmp(p->name, name)==0)
      return p;
    p = p->sibling;
  }
  return 0;
}

int insert_child(NODE *parent, NODE *q)
{
  NODE *p;
  printf("insert NODE %s into parent's child list\n", q->name);
  p = parent->child;
  if (p==0)
    parent->child = q;
  else{
    while(p->sibling)
      p = p->sibling;
    
    p->sibling = q;
  }
  q->parent = parent;
  q->child = 0;
  q->sibling = 0;
}

void tokenize(char *pathname){
	char *s;
	n = 0;
	strcpy(gpath, pathname);
	s = strtok(gpath, "/");
	while(s){
		//printf("%s", s);

		name[n] = s;
		s = strtok(0, "/");
		n++;
	}
}

NODE *path2node(char *pathname){
	if(dname[0] == '/')			//if pathname is absolute
		start = root;
	else								//if pathname is relative
		start = cwd;

	tokenize(pathname);	
	
	NODE *p = start;
	for(int i = 0; i < n; i++){		
		if(strcmp(name[i],".") == 0){
			p = p;
			continue;
		}
		if(strcmp(name[i],"..") == 0){
			p = p->parent;
			continue;
		}
		
		p = search_child(p, name[i]);
		
		if(p == 0){
			printf("Error: path not found\n");
			return 0;
		}
	}
	return p;
}

/***************************************************************
 This mkdir(char *name) makes a DIR in the current directory
 You MUST improve it to mkdir(char *pathname) for ANY pathname
****************************************************************/

int mkdir(char *pathname)
{
	NODE *p, *q;
	printf("mkdir: name=%s\n", pathname);
	
	dbname(pathname);
	printf("dname=%s bname=%s\n", dname, bname);
	//check base name for invalid directory names
	if (!strcmp(bname, "/") || !strcmp(bname, ".") || !strcmp(bname, "..")){
		printf("can't mkdir with %s\n", bname);
		return -1;
	}

	//set start to the location for node insertion
	start = path2node(dname);	
	
	p = search_child(start, bname);
	if (p){
		printf("name %s already exists, mkdir FAILED\n", bname);
		return -1;
	}

	q = (NODE *)malloc(sizeof(NODE));
	q->type = 'D';
	strcpy(q->name, bname);
	insert_child(start, q);
	printf("---- mkdir %s OK ----\n", bname);
	 
	return 0;
}

int rmdir(char *pathname)
{
	NODE *p, *q;
	printf("rmdir: name=%s\n", pathname);
	
	dbname(pathname);
	printf("dname=%s bname=%s\n", dname, bname);
	//check base name for invalid directory names
	if (!strcmp(bname, "/") || !strcmp(bname, ".") || !strcmp(bname, "..")){
		printf("can't rmdir with %s\n", bname);
		return -1;
	}
	
	//set start to the location for node insertion
	start = path2node(dname);

	p = search_child(start, bname);
	if (!p){
		printf("name %s does not exist, rmdir FAILED\n", bname);
		return -1;
	}
	
	//check to make sure DIR is empty
	if(p->child != 0){
		printf("---- rmdir : dir \"%s\" not empty ----\n", bname);
		return -1;
	}

	//remove parent's pointer to dir, replace with pointer to sibling
	q = p->parent;
	q->child = p->sibling;
	
	//deallocate memory
	free(p);
	printf("---- rmdir %s OK ----\n", bname);
	 
	return 0;
}

int creat(char *pathname){
	NODE *p, *q;
	printf("creat: name=%s\n", pathname);
	
	dbname(pathname);

	//check base name for invalid file names
	if (!strcmp(bname, "/") || !strcmp(bname, ".") || !strcmp(bname, "..")){
		printf("can't creat with %s\n", bname);
		return -1;
	}

	//set start to the location for node insertion
	start = path2node(dname);
	
	p = search_child(start, bname);
	if (p){
		printf("name %s already exists, creat FAILED\n", bname);
		return -1;
	}

	q = (NODE *)malloc(sizeof(NODE));
	q->type = 'F';
	strcpy(q->name, bname);
	insert_child(start, q);
	printf("---- creat %s OK ----\n", bname);
	 
	return 0;
}

int rm(char *pathname){
	NODE *p, *q;
	printf("rm: name=%s\n", pathname);
	
	dbname(pathname);

	//set start to the location for node deletion
	start = path2node(dname);

	p = search_child(start, bname);
	if (!p){
		printf("name %s does not exist, rm FAILED\n", bname);
		return -1;
	}
	
	//check to make sure type is file
	if(p->type != 'F'){
		printf("---- rm : \"%s\" is not a file ----\n", bname);
		return -1;
	}

	//remove parent's pointer to file
	q = p->parent;
	q->child = 0;
	//deallocate memory
	free(p);
	printf("---- rm %s OK ----\n", bname);
	 
	return 0;
}

int cd(char *pathname){
	NODE *p;
	p = path2node(pathname);
	if(p->type == 'D')
		cwd = p;
	else{
		printf("---- cd : %s is not a directory ----\n", p->name);
		return -1;
	}
	return 0;
}

int ls(char *pathname)
{
	NODE *p;
	dbname(pathname);
	p = path2node(pathname);
	p = p->child;
	
	while(p){
		printf("[%c %s] ", p->type, p->name);
		p = p->sibling;
	}
	printf("\n");
}

int pwdhelper(NODE *p){
	if(p == root){
		printf("/");
	}
	else{
		pwdhelper(p->parent);
		printf("%s/", p->name);
	}
}

int pwd(){
	NODE *p = cwd;			//init p to cwd
	pwdhelper(p);			//follow the parentPtr to the parent node until root
	printf("\n");
}

int quit()
{
  printf("Program exit\n");
  exit(0);
}

int initialize()
{
	root = (NODE *)malloc(sizeof(NODE));
	strcpy(root->name, "/");
	root->parent = root;
	root->sibling = 0;
	root->child = 0;
	root->type = 'D';
	cwd = root;
	printf("Root initialized OK\n");
}

int (*func_ary[])(char *) = {(int (*) ())mkdir, ls, quit, rmdir, cd, creat, rm, pwd};

int main()
{
	int index;
	
	initialize();

	while(1){
		printf("Command : ");
		fgets(line, 128, stdin);		// get input line
		line[strlen(line)-1] = 0;		// kill \n at end
		*pathname = 0;						// init pathname
		
		sscanf(line, "%s %s", command, pathname); // extract command pathname
		printf("command=%s pathname=%s\n", command, pathname);

		if (command[0]==0) 
			continue;

		index = findCmd(command);
		
		int r = func_ary[index](pathname);
	}
}

