//ERIC JOHNSON
//PROJECT 10
//race condition 1:


//assume two threads have called alloc as part of what their running: 

//thread A and thread B both reach the if statement checking if the block is free
//They both confirm the block is free, 
//and both write to the block and return a int pointer to the same block num
//This can cause problems as two threads might have conflicting use for alloc


//to fix, put a lock after the bread and unlock after the set_free, so the bit is marked as taken to the
//other thread

int alloc(void){

    unsigned char emptyblock[BLOCK_SIZE] = {0};
    bread(2, emptyblock);
	//RACE HERE
    int emptybit = find_free(emptyblock); //problem occurs here if both read the same emptybit
    if (emptybit == -1){ //<<RACE OCCURS HERE
        return -1;
    }
    set_free(emptyblock, emptybit, 1);
	//UNLOCK HERE
    bwrite(2, emptyblock);

    return emptybit;

}


//Race condition 2:

//In ialloc, assume 2 threads are calling Ialloc to allocate a new inode
//Thread A and thread B both read from the inode map block, and both find the same free bit/inode
//This causes thread A and thread B to write to the same spot in memory, 
//and eventually return the same inode pointer location.
//This can cause problems with two threads believing they have a unique inode in memory

//To fix, include a lock after the bread, and unlock after the set_free
//This prevents them from returning the same inode, as the node will now be marked taken.

struct inode * ialloc(void){
    unsigned char emptyblock[BLOCK_SIZE] = {0};
    bread(1, emptyblock);
	//LOCK HERE
    int emptybit = find_free(emptyblock);
    if (emptybit == -1){ //<<RACE OCCURS HERE
        return NULL;
    }
    set_free(emptyblock, emptybit, 1);
	//UNLOCK HERE
    bwrite(1, emptyblock);
	//...
}

//RACE CONDITION 3:

//Assume 2 threads are both calling iput, and both see the inode has a reference count of 1
//they both decrement it by 1, changing the value to a -1
//This causes the if statement to write to fail, and the function fails as now neither can write the inode.

//to fix, place a lock around the whole thing, OR place a lock before the decrement and unlock after the if
//locking the whole thing is more "sound" as it means the second thread to call will do the proper "if == 0" check
//but seeing as how this function is void and doesn't need to return anything, the latter works just as well

void iput(struct inode *in){

	//LOCK HERE
    if (in->ref_count == 0){
        return;
    }
	//OR LOCK HERE
    in->ref_count -= 1;
    if (in->ref_count == 0){
        write_inode(in);
    }
	//UNLOCK HERE

}

//RACE CONDITION 4:

//Assume 2 threads both want to write different data to the same inode
//Thread A gets priority and writes down size, but then thread B gets priority and overrides it
//Thread A's size was bigger than thread B, so when later on their writing to the blocks,
//thread A's blocks ovveride thread B's due to wishy washy writing, and causes the inode to have a larger size
//than what is listed, causing problems

//to fix, put a lock before the write statements, and unlock after the bwrite

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

	//LOCK HERE
	//>>RACES OCCUR EVERYWHERE AFTER THIS
    write_u32(block + block_offset_bytes + 0, in->size); //size
    write_u16(block + block_offset_bytes + 4, in->owner_id);//owner_id
    write_u8(block + block_offset_bytes + 6, in->permissions);  //permissions
    write_u8(block + block_offset_bytes + 7, in->flags); //flags
    write_u8(block + block_offset_bytes + 8, in->link_count); //link_count



    for (int i=0; i < 16; i++){    //sets everything to empty block?
        write_u16(block + block_offset_bytes + 9+(i*2), in->block_ptr[i]);
    }

    bwrite(block_num, block); //writes the filled out block back to disk memory
	//UNLOCK HERE

}