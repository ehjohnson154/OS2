
#ifndef IALLOC_H
#define IALLOC_H


int ialloc(void);

//project 6, incore inodes
#define BLOCK_SIZE 4096
#define INODE_SIZE 64
#define INODE_FIRST_BLOCK 3

#define INODES_PER_BLOCK (BLOCK_SIZE / INODE_SIZE)

#define MAX_SYS_OPEN_FILES 64
#define INODE_PTR_COUNT 16

#define INODE_PTR_COUNT 16

struct inode {
    unsigned int size;
    unsigned short owner_id;
    unsigned char permissions;
    unsigned char flags;
    unsigned char link_count;
    unsigned short block_ptr[INODE_PTR_COUNT];

    unsigned int ref_count;  // in-core only
    unsigned int inode_num;
};

//project 6
// struct inode *find_incore_free(void);
// struct inode *find_incore(unsigned int inode_num);
// void read_inode(struct inode *in, int inode_num);
// void write_inode(struct inode *in);
// struct inode *iget(int inode_num);

#endif