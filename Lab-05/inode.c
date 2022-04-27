#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <time.h>

#include "inode.h"
#include "superblock.h"


inode* read_inode(int fd, int inode_number){
	inode* ip = malloc(sizeof(inode));
	int currpos=lseek(fd, INODE_OFFSET + inode_number * sizeof(inode), SEEK_SET);
	if(currpos<0){
		printf("Error: lseek()\n");
		return NULL;
	}
	
	//read inode from disk
	int ret = read(fd, ip, sizeof(inode));
	if(ret != sizeof (inode) ){
		printf("Error: read()\n");
		return NULL;
	}
	return ip;
}

void print_inode_info(inode* ip){
	printf("The inode information: \n");
	printf("[1] inode_number:\t\t%d\n", ip->inode_number);
	printf("[2] creation_time:\t\t%s", ctime(& ip->creation_time));
	printf("[3] file_type:\t\t\t%d\n", ip->file_type);
	printf("[4] file_size:\t\t\t%d\n", ip->file_size);
	printf("[5] block_number:\t\t%d\n", ip->block_number);
	printf("[6] direct_block[0]:\t\t%d\n", ip->direct_block[0]);
	printf("[7] direct_block[1]:\t\t%d\n", ip->direct_block[1]);
	printf("[8] indirect_block:\t\t%d\n", ip->indirect_block);
	printf("[9] sub_file_number:\t\t%d\n", ip->sub_file_number);
}

void print_inode_region(int fd, int inode_number){
	printf("The inode region on disk (Hexadecimal):\n");
	unsigned int buf[sizeof(inode) / 4];
	int currpos = lseek(fd, INODE_OFFSET + inode_number * sizeof(inode), SEEK_SET);
	read(fd, buf, sizeof(inode));
	int i;
	for (i = 0; i < sizeof(inode) / 4; i++){
		printf("[%d] %04x\n", i+1, buf[i]);
	}
}

void print_directory_mappings(int fd, int inode_number)
{
	inode* ip;
	ip = read_inode(fd, inode_number);
	if(ip->file_type != DIR)
	{
		printf("Wrong path!\n");
		return;
	}

	DIR_NODE* p_block = (DIR_NODE* )malloc(BLOCK_SIZE);
	// Consider that SFS only supports at most 100 inodes so that only direct_blk[0] will be used,
	// the implementation is much easier
	int block_number = ip->direct_block[0];
	int currpos=lseek(fd, DATA_BLOCK_OFFSET + block_number * BLOCK_SIZE, SEEK_SET);
	read(fd, p_block, BLOCK_SIZE);

	int file_idx = 0;
	printf("file_name \t inode_number\n");
	for(file_idx = 0; file_idx < ip->sub_file_number; file_idx++)
	{
		printf("%s \t\t %d\n", p_block[file_idx].file_name, p_block[file_idx].inode_number);
	}
	free(p_block);
}
