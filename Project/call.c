#include "call.h"
const char *HD = "HD";

superblock* read_sb(int fd){
	superblock* sb = malloc(sizeof(superblock));
	int currpos=lseek(fd, SUPER_BLOCK_OFFSET, SEEK_SET);
	if(currpos<0){
		printf("Error: lseek()\n");
		return NULL;
	}
	
	//read superblock from disk
	int ret = read(fd, sb, sizeof(superblock));
	if(ret != sizeof (superblock) ){
		printf("Error: read()\n");
		return NULL;
	}
	return sb;
}

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



int gen_next_inode(int fd, int i_num,char* dir_name){
	int next_inode =-1;
	inode* ip;
	ip = read_inode(fd, i_num);
	if(ip->file_type != DIR)
	{
		printf("Wrong path!\n");
		return -1;
	}

	DIR_NODE* p_block = (DIR_NODE* )malloc(BLOCK_SIZE);

	int block_number = ip->direct_block[0];
	int currpos=lseek(fd, DATA_BLOCK_OFFSET + block_number * BLOCK_SIZE, SEEK_SET);
	read(fd, p_block, BLOCK_SIZE);

	int file_idx = 0;

	for(file_idx = 0; file_idx < ip->sub_file_number; file_idx++)
	{
		if(strcmp(p_block[file_idx].file_name,dir_name)==0){
		next_inode = p_block[file_idx].inode_number;
		break;}
	}
	free(p_block);
	free(ip);
	return next_inode;
}


int open_t(char *pathname)
{
	int inode_number ;
	// write your code here.

	//read the superblock
	int fd = open("./HD",O_RDONLY);

	superblock* sb = read_sb(fd);
	inode *root = read_inode(fd,0);
	if(root ==NULL)return -1;


	inode_number =0;
	char *dir_name = strtok(pathname,"/");
	while(dir_name != NULL){

		inode_number = gen_next_inode(fd,inode_number,dir_name);
		
		if(inode_number<0) return -1;
		dir_name = strtok(NULL, "/");
	}
	close(fd);
	return inode_number;
}


int read_t(int inode_number, int offset, void *buf, int count)
{
	int read_bytes;
	// write your code here.

	if(inode_number <0){
		return -1;
		}
	int fd = open("./HD",O_RDONLY);
	superblock* sb = read_sb(fd);
	inode * inode_toread = read_inode(fd,inode_number);
	if(inode_toread ==NULL){
		return -1;
		}
	if(inode_toread->file_type!=0){
		return -1;
	}
	int total = count+offset;
	if(total <= inode_toread->file_size){
		read_bytes=count;
	}else{
		read_bytes = inode_toread->file_size-offset;
		}
	if(read_bytes<0) {
		read_bytes=0;
		}
	if(read_bytes>0){
			int point =0;
			int end,start;
			start = offset/BLOCK_SIZE;
			end = (offset+read_bytes)/BLOCK_SIZE;
			offset = offset - start*BLOCK_SIZE;

			if(start==0 && end>=0){
				printf("going to read dir blk 0 pt: %d\n",point);
				int size = sb->block_size-offset;
				if(size >=read_bytes) size = read_bytes;
				lseek(fd, DATA_BLOCK_OFFSET+inode_toread->direct_block[0]*BLOCK_SIZE+offset, SEEK_SET);
				read(fd,(char*)buf+point,size);
				point = point+size;
				offset=0;
				printf("read dir blk 0 cur pt: %d\n \n",point);
			}	

			if(start<=1 && end >=1){
				printf("going to read dir blk 1 pt: %d\n",point);
			int t = read_bytes-point;
			int size = sb->block_size-offset;
			if(size >=t){
				size=t;
				}
			lseek(fd, DATA_BLOCK_OFFSET+inode_toread->direct_block[1]*BLOCK_SIZE+offset,SEEK_SET);
			read(fd,(char*)buf+point,size);
			point=point+size;
			offset=0;
				printf("read dir blk 1 cur pt: %d\n \n",point);
			}

			int read_block,indr_ind;

			for(int read_block=(start<2)?2:start; read_block<=end;read_block++){
				indr_ind = read_block-2;
				printf("going to read indir blk %d pt %d\n",indr_ind,point);
                lseek(fd, DATA_BLOCK_OFFSET+inode_toread->indirect_block*BLOCK_SIZE+indr_ind*sizeof(int), SEEK_SET);
                read(fd,&indr_ind,sizeof(int));
				printf("indir blk at %d\n",indr_ind);
                int t = read_bytes-point;
                int size = BLOCK_SIZE-offset;
				if(size >=t)size=t;
                lseek(fd, DATA_BLOCK_OFFSET+indr_ind*BLOCK_SIZE+offset,SEEK_SET);
                read(fd,(char*)buf+point,size);
                point=point+size;
                offset=0;
				printf("read indir blk %d cur pt %d\n \n", read_block-2, point);
			}

		}
	close(fd);
	return read_bytes; 
}

// you are allowed to create any auxiliary functions that can help your implementation. But only "open_t()" and "read_t()" are allowed to call these auxiliary functions.