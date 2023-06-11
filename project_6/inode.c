#include <stdio.h>

#include "inode.h"
#include "free.h"
#include "image.h"
#include "block.h"
#include "pack.h"


//old ialloc

//should find the lowest numbered free inode from the inode map, mark it as allocated, 
//and return that number. If there are no free inodes, it should return -1. 
//(Later on we'll change what this function returns, but for now this is fine.)

// In order to do this, it will have to call bread() to get the inode map, 
// then find_free() to locate a free inode, 
// then set_free() to mark it as non-free, then bwrite() to save the inode back out to disk.

// int ialloc(void){
//     unsigned char emptyblock[BLOCK_SIZE] = {0};
//     bread(1, emptyblock); //What is our block num?
//     int emptybit = find_free(emptyblock);
//     if (emptybit == -1){
//         return -1;
//     }
//     set_free(emptyblock, emptybit, 1);
//     bwrite(1, emptyblock);

//     return emptybit;

// }

//project 6
//------------

// int block_num = inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
// int block_offset = inode_num % INODES_PER_BLOCK;
// int block_offset_bytes = block_offset * INODE_SIZE;
// int flags = read_u8(block + block_offset_bytes + 7);

static struct inode incore[MAX_SYS_OPEN_FILES] = {0};

void reset_incore(void) {
    for(int i = 0; i < MAX_SYS_OPEN_FILES; i++) {
        incore[i].ref_count = 0;
    }
}

void fill_incore(void) {
    for(int i = 0; i < MAX_SYS_OPEN_FILES; i++) {
        incore[i].ref_count = 1;
    }
}



//----------------


//getting inode
struct inode * ialloc(void){
    unsigned char emptyblock[BLOCK_SIZE] = {0};
    bread(1, emptyblock); //What is our block num?
    int emptybit = find_free(emptyblock);
    if (emptybit == -1){
        //return -1;
        return NULL;
    }
    set_free(emptyblock, emptybit, 1);
    bwrite(1, emptyblock);
    //return emptybit;

// Let's add this to the end of the existing ialloc() code:

//     Save the inode number of the newly-allocated inode (returned by find_free())
//     If none are free: Return NULL //isnt this already done sort of?

//     Get an in-core version of the inode (iget())
    struct inode *inode_alloc = iget(emptybit);
    //     If not found: Return NULL
    if (inode_alloc == NULL ){
        return NULL;
    }

//     Initialize the inode:
//         Set the size, owner ID, permissions, and flags to 0.
//         Set all the block pointers to 0.
//         Set the inode_num field to the inode number (from find_free())
//     Save the inode to disk (write_inode())
//     Return the pointer to the in-core inode.
    inode_alloc->inode_num = emptybit;
    inode_alloc->size = 0;
    inode_alloc->owner_id = 0;
    inode_alloc->permissions = 0;
    inode_alloc->flags = 0;
    //use for loop to set all 16 block pointers to 0
    //CHECK THIS
    for (int i = 0; i < 16; i++){
        inode_alloc->block_ptr[i] = 0;
    }
 //does NOT
    write_inode(inode_alloc);
    return inode_alloc; //does this return it to incore...?

}

//project6 incore inodes
//--------------------------------

struct inode *find_incore_free(void){
//This finds the first free in-core inode in the incore array. 
//It returns a pointer to that in-core inode or NULL if there are no more free in-core inodes.

//1. loop through icore[] array.
//2. check each inode struct if reference count is 0, it is free
//3. return if free, loop if not, return null if end of loop

    for (int i=0; i < MAX_SYS_OPEN_FILES; i++){ //do a for loop of MAX_OPEN_SYS instead!
        struct inode * check = &(incore[i]);
        if(check->ref_count == 0){
            return check;
        }
        
    }
    return NULL;

    
}

struct inode *find_incore(unsigned int inode_num){
//This finds an in-core inode record in the incore array by the inode number. 
//It returns a pointer to that in-core inode or NULL if it can't be found.

//1. loop through icore array []
//2.check for non-0 reference count AND inode num matches passed num

    for (int i=0; i < MAX_SYS_OPEN_FILES; i++){//CHANGE TO FOR LOOP
        struct inode * check = &(incore[i]);
        //printf("ref count of check %d\n", check->ref_count);
        if((check->ref_count != 0) && (check->inode_num == inode_num)){
            //printf("returning icore $ %d\n", check->inode_num);
            return check;
        }
        
    }
    //printf("returning null incore\n");
    return NULL;

}

void read_inode(struct inode *in, int inode_num){
//This takes a pointer to an empty struct inode that you're going to read the data into. 
//The inode_num is the number of the inode you wish to read from disk.

    //1. find inode in memory using offsets
    //2. read information into in.
    
    int block_num = inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
    int block_offset = inode_num % INODES_PER_BLOCK;
    int block_offset_bytes = block_offset * INODE_SIZE;

    //do it into dummy block
    //inode block of block size is equal to
    //dummy block of memory, know block number
    unsigned char block[BLOCK_SIZE] = {0};
    bread(block_num, block);

    in->size = read_u32(block + block_offset_bytes + 0); //use 32 because size 4.
    in->owner_id = read_u16(block + block_offset_bytes + 4); //offset 4 because size is 4
    in->permissions = read_u8(block + block_offset_bytes + 6);
    in->flags = read_u8(block + block_offset_bytes + 7);
    in->link_count = read_u8(block + block_offset_bytes + 8);


    for (int i=0; i < 16; i++){//    //does this work to search through block pointers? //probably not?
        in->block_ptr[i] = read_u16(block + block_offset_bytes + 9+(2*i));
    }


}

void write_inode(struct inode *in){
//This stores the inode data pointed to by in on disk. 
//The inode_num field in the struct holds the number of the inode to be written.
    int inode_num = 0;
    inode_num = in->inode_num;
    int block_num = inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
    int block_offset = inode_num % INODES_PER_BLOCK;
    int block_offset_bytes = block_offset * INODE_SIZE;

    unsigned char block[BLOCK_SIZE] = {0};
    bread(block_num, block);


    write_u32(block + block_offset_bytes + 0, in->size); //size
    write_u16(block + block_offset_bytes + 4, in->owner_id);//owner_id
    write_u8(block + block_offset_bytes + 6, in->permissions);  //permissions
    write_u8(block + block_offset_bytes + 7, in->flags); //flags
    write_u8(block + block_offset_bytes + 8, in->link_count); //link_count



    for (int i=0; i < 16; i++){    //sets everything to empty block?
        write_u16(block + block_offset_bytes + 9+(i*2), in->block_ptr[i]);
    }

    bwrite(block_num, block); //writes the filled out block back to disk memory

}

struct inode *iget(int inode_num){
//Return a pointer to an in-core inode for the given inode number, or NULL on failure.
    // int block_num = inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
    // int block_offset = inode_num % INODES_PER_BLOCK;
    // int block_offset_bytes = block_offset * INODE_SIZE;

    // Search for the inode number in-core (find_incore())
    struct inode *incore = find_incore(inode_num);
    if(incore != NULL){
    // If found:
    //     Increment the ref_count
    //     Return the pointer
        incore->ref_count += 1;
        return incore;
    }
  
    // Find a free in-core inode (find_incore_free())
    struct inode *free_incore = find_incore_free();
    if (free_incore == NULL){
    // If none found:
    //     Return NULL
        return NULL;
    }

    // Read the data from disk into it (read_inode())
    // Set the inode's ref_count to 1
    // Set the inode's inode_num to the inode number that was passed in
    // Return the pointer to the inode
    read_inode(free_incore, inode_num);
    free_incore->ref_count = 1;
    free_incore->inode_num = inode_num;
    return free_incore;


}

void iput(struct inode *in){
    //decrement the reference count on the inode. If it falls to 0, write the inode to disk.


    // If ref_count on in is already 0:
    //     Return
    if (in->ref_count == 0){
        return;
    }
    in->ref_count -= 1;
    if (in->ref_count == 0){
        write_inode(in);
    }
    // Decrement ref_count
    // If ref_count is 0:
    //     Save the inode to disk (write_inode())

}