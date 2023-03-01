/************** lab5base.c file ******************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <string.h>

typedef struct ext2_super_block SUPER;
typedef struct ext2_group_desc  GD;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;

SUPER *sp;
GD    *gp;
INODE *ip;
DIR   *dp;   

#define BLK 1024

int fd;             // opened vdisk for READ
int inodes_block;   // inodes start block number

char gpath[128];    // token strings
char *name[32];
int n;

int get_block(int fd, int blk, char *buf)
{
   lseek(fd, blk*BLK, 0);
   read(fd, buf, BLK);
}   

int search(INODE *ip, char *name)
{
   // Chapter 11.4.4  int i; 
   // Exercise 6
   char *cp;                     //char pointer
   char local_buf[BLK], tempname[256];

   for(int i = 0; i < 12; i++){
      if(ip->i_block[i]){
         int blk = ip->i_block[i];
         get_block(fd,blk,local_buf);
         dp = (DIR*) local_buf;
         cp = local_buf;

         printf("inode# rec_len name_len name\n");
         while(cp < local_buf + BLK){
            strncpy(tempname, dp->name, dp->name_len);   //copy name to tempname
            tempname[dp->name_len] = 0;                  //null-terminate
            printf("%6d %6d %6d %s\n", dp->inode, dp->rec_len, dp->name_len, tempname);

            if(strcmp(tempname, name) == 0){
               return dp->inode;
            }
            cp += dp->rec_len;
            dp = (DIR*) cp;
         }
         
      }
   }

   return 0;
}


int tokenize(char *pathname)
{
   char *s;
   //strcpy(gpath, pathname);   // copy into global gpath[]
   s = strtok(pathname, "/");    
   n = 0;
   while(s){
      name[n++] = s;          // token string pointers   
      s = strtok(0, "/");
   }
   name[n] = 0;               // arg[n] = NULL pointer 
   return 0;
}

char *disk = "vdisk";

int main(int argc, char *argv[])   // a.out pathname
{
   char buf[BLK];  // use more bufs if you need them

   //open vdisk for READ: print fd value
   fd = open(disk, O_RDWR);
   printf("fd: %d\n", fd);

   //read SUPER block #1 to verify EXT2 fs : print s_magic in HEX
   get_block(fd, 1, buf);
   sp = (SUPER*)buf;
   printf("s_magic: 0x%x\n", sp->s_magic);

   //read GD block #2 to get inodes_block=bg_inode_table: print inodes_block 
   get_block(fd,2,buf);
   gp = (GD*)buf;
   inodes_block = gp->bg_inode_table;
   printf("inodes block: %d\n", inodes_block);

   //read inodes_block into buf[] to get root INODE #2 : set ino=2 
   get_block(fd, inodes_block, buf);
   int ino = 2;
   INODE *ip = (INODE *)buf + 1;
   
   //tokenize pathame=argv[1]);
   tokenize(argv[1]);

   for (int i = 0; i<n; i++){
      printf("===========================================\n");
      printf("search name[%d]=%s in ino=%d\n", i, name[i], ino);
      printf("i=%d i_block[%d]=%d\n", i, i, ip->i_block[i]);
      ino = search(ip, name[i]);

      if (ino==0){
         printf("name %s does not exist\n", name[i]);
         exit(1);
      }
      // MAILman's algrithm:
      int blk = (ino-1) / 8 + inodes_block;
      int offset = (ino-1) % 8;
      get_block(fd, blk, buf);

      char trash;
      printf("found %s : ino = %d\n", name[i], ino);
      printf("blk = %d offset = %d\n", blk, offset);
      printf("enter a key : ");
      scanf("%c", &trash);

      ip = (INODE *)buf + offset;
   } 

   printf("****************  DISK BLOCKS  *******************\n");
   for(int i = 0; i < 14; i++){
      if(ip->i_block[i]){
         printf("block[%2d] = %d\n", i, ip->i_block[i]);
      }
   }
   for (int i = 0; i < 14; i++){
      if(ip->i_block[i]){
         if(i == 0)
            printf("================ Direct Blocks ===================\n");
         if(i < 12){
            printf("%2d ", ip->i_block[i]);
         }

         if(i == 12){
            printf("\n===============  Indirect blocks   ===============\n");
            char local_buf[BLK];
            get_block(fd, ip->i_block[i], local_buf);
            int *indirect_block = (int*) local_buf;

            for(int j = 0; j < 256; j++){
               if(indirect_block[j]){
                  printf("%2d ", indirect_block[j]);
               }
               else
                  break;
            }
            printf("\n");
         }

         if(i == 13){
            printf("===========  Double Indirect blocks   ============\n");
            char local_buf[BLK];
            get_block(fd, ip->i_block[13], local_buf);
            int* indirect_block = (int*) local_buf;
            
            for(int j = 0; j < 256; j++){
               char local_buf2[BLK];
               get_block(fd, indirect_block[j], local_buf2);
               int* double_indirect_block = (int*) local_buf2;

               for(int k = 0; k < 256; k++){
                  if(double_indirect_block[k])
                     printf("%2d ", double_indirect_block[k]);
                  else
                     break;
               }
            }
            printf("\n");
         }
      }
   }
}