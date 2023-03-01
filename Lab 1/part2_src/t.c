// please put your main and code in this file. All contained.
// Use the provided mk script to compile your code. You are welcome to change the mk script if you'd like
// but know that is how we will be batch compiling code using mk on the back end so your code will need to be able
// to run when that bash script is ran.
// Most code can be grabbed from t.c in root of the github repo.

#include <stdio.h>
#include <fcntl.h>

#include <sys/types.h>
#include <unistd.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

struct partition {
	u8 drive;             // drive number FD=0, HD=0x80, etc.

	//these three can be ignored
	u8  head;             // starting head 
	u8  sector;           // starting sector
	u8  cylinder;         // starting cylinder

	u8  sys_type;         // partition type: NTFS, LINUX, etc.

	u8  end_head;         // end head 
	u8  end_sector;       // end sector
	u8  end_cylinder;     // end cylinder

	u32 start_sector;     // starting sector counting from 0 
	u32 nr_sectors;       // number of of sectors in partition
};

char *dev = "vdisk";
int fd;

// read a disk sector into char buf[512]
int read_sector(int fd, int sector, char *buf)
{
    lseek(fd, sector*512, SEEK_SET);  // lssek to byte sector*512
    read(fd, buf, 512);               // read 512 bytes into buf[ ]
}

int main(){
	struct partition *p;
	char buf[512];

	fd = open(dev, O_RDONLY);   // open dev for READ
	// read(fd, buf, 512);         // read 512 bytes of fd into buf[ ] 
	read_sector(fd, 0, buf);    // read in MBR at sector 0    

	//Write C code to let p point at Partition 1 in buf[];
	p = (struct partition *) &buf[446];
	
	//print P1's start_sector, nr_sectors, sys_type;
	printf("%10s%10s%10s\n", "Start", "Sectors", "Type");
	printf("%10d%10d%10d\n", p->start_sector, p->nr_sectors, p->sys_type);
	
	//Write code to print all 4 partitions;
	for(int i = 0; i < 3; i++){
		p++;
		printf("%10d%10d%10d\n", p->start_sector, p->nr_sectors, p->sys_type);
	}
	
	// ASSUME P4 is EXTEND type:
	int extStart = p->start_sector;
	printf("\nextstart=%d\n\n", extStart);
	int localMBR = extStart;
	
	int count = 5;
	while(p->sys_type != 0){
		read_sector(fd, localMBR, buf);
		p = (struct partition *) &buf[446];
		
		// partition table of localMBR in buf[ ] has 2 entries:
		//print entry 1's start_sector, nr_sector;
		printf("LocalMBR:\n");
		printf("%10s%10s\n", "Start", "Sectors");
		printf("%10d%10d\n", p->start_sector, p->nr_sectors);
		int start_sector = p->start_sector;
		int sectors = p->nr_sectors;
		
		//switch to entry 2
		p++;
		
		//compute and print P5's begin, end, nr_sectors
		printf("Partition %d:\n", count);
		printf("%10s%10s%10s\n", "Begin", "End", "Sectors");
		printf("%10d%10d%10d\n", localMBR+start_sector, localMBR+start_sector+sectors-1, sectors);
		
		//compute and print next localMBR sector;
		localMBR = extStart + p->start_sector;
		count++;
	}
}

