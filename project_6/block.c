#include <unistd.h>
#include <stdio.h>
#include "image.h"
#include "free.h"
#include "block.h"
#include "inode.h"
//#include "free.c"


// bread() should take a block number and a pointer to a block-sized unsigned char buffer to load the data into. 
// For convenience, it should also return block at the end.
unsigned char *bread(int block_num, unsigned char *block){

    //compute how far the point is

    //lseek to move pointer
    lseek(image_fd, block_num * 4096, SEEK_SET );
    //needs to be pointer
    read(image_fd, block, 4096);
    //read to actually read from that block
    //read the WHOLE block

    return block;
}

//bwrite() takes a block number and a pointer to the data to write.
void bwrite(int block_num, unsigned char *block){
    lseek(image_fd, block_num * 4096, SEEK_SET );
    write(image_fd, block, 4096);
}

//allocate a previous-free data block from the block map.
//it's going to be the same as ialloc(), 
//except we're going to pass in the free data block instead of the free inode block.

//getting block
int alloc(void){

    unsigned char emptyblock[BLOCK_SIZE] = {0};
    //printf("%d\n", emptyblock);
    bread(2, emptyblock);
    int emptybit = find_free(emptyblock);
    //printf("%d\n", emptybit);
    if (emptybit == -1){
        return -1;
    }
    set_free(emptyblock, emptybit, 1);
    bwrite(2, emptyblock);

    return emptybit;

}