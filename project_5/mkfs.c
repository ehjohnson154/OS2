#include <unistd.h>
#include "block.h"
#include "image.h"
#include "mkfs.h"
#include "inode.h"
//#include "dirbasename.c"

#define ROOT_INODE_NUM 0 //?
#define DIR_ENTRY_SIZE 32 


void mkfs(void){

    // this would go in main, but for now test framework: image_open(image_fd);

    //create array of unsigned char
    unsigned char arr[4194304]; //4096 * 1024
    
    //int byte_num = num / 8; 
    //Write 1024 blocks of all zero bytes, sequentially, using the write() syscall. 
    for (int i = 0; i < 4194304; i++){
        arr[i] = 0;
    }
    write(image_fd, arr, 4096);
    //What is the buffer? //ANS: is the arr
    //How do you write 1024 blocks?
    //How do you subsequently mark those data blocks for alloc?

    //This will make a 4096*1024-byte image, or 4 MB.

    //Mark data blocks 0-6 as allocated by calling alloc() 7 times.
    for(int i = 0; i < 7; i++){
        alloc(); //sets them up as in use
    }



    //project 7: directories

    //create the root directory
    // broken down:

    // Call ialloc() to get a new inode.
//     struct inode *root_inode = ialloc();

//     // We'll need the inode_num later to add it to the directory!
//     int inode_num = root_inode->inode_num;

//     // Call alloc() to get a new data block.
//     // This will hold the directory entries.
//     // Be sure to note the returned block number because we're going to need to write to it in just a minute!
//     int root_block_num = alloc();

//     // Initialize the inode returned from ialloc(), above.
//     //     flags needs to be set to 2.

//     //replace these magic numbers with macros (#defines)
//     root_inode->flags = 2; //2 flags indicate directories

//     //     size needs to be set to the byte size of the directory. 
//     //Since we have two entries (. and ..) and each is 32 bytes, the size must be 64 bytes.
//     root_inode->size =64;//#directory entry size * 2

//     //     block_ptr[0] needs to point to the data block we just got from alloc(), above.
//     root_inode->block_ptr[0] = root_block_num;

//     // Make an unsigned char block[BLOCK_SIZE] array that you can populate with the new directory data.
//     // We're going to pack the . and .. directory entries in here.
//     unsigned char block[BLOCK_SIZE];



//     // Add the directory entries. You're going to need to math it out.
//     // You know the first two-byte value is the inode number, 
//     //so you can pack that in with write_16(). 
//     //Remember that for the root inode, both the . and .. 
//    // inode numbers are the same as the root inode itself 
//     //(i.e. the inode_num you got back from ialloc()).


//     unsigned char *root_0 = block+ DIR_ENTRY_SIZE * 0; //can place this in to block instead
//     unsigned char *root_1 = block +DIR_ENTRY_SIZE * 1;

//     write_16(root_0, inode_num); //what is the addr? block[0]? //.
//     //what is the second directory
//     write_16(root_1, inode_num); //..

//     // The next up-to-16 bytes are the file name. You can copy that in with strcpy(). 
//     //(You might have to cast some arguments to char*.)
//     //inode size is 2.
//     unsigned char *root_0_name = root_0 + 2;
//     unsigned char *root_0_name = root_1+2;

//     strcpy((char *) root_0_name, ".");
//     strcpy((char *) root_1_name, "..");


//     // You have to do this process for both . and .. entries.
//     // Compute the offsets into your in-memory block and copy the data there.

//     //Write the directory data block back out to disk with bwrite().
//     bwrite(root_block_num, block); //pretty sure?

//     //Call iput() to write the new directory inode out to disk and free up the in-core inode.
//     iput(root_inode); //pretty sure?

}


// struct directory *directory_open(int inode_num){ 

//     //     Use iget() to get the inode for this file.
//     struct inode* directory_inode = iget(inode_num);

//     // If it fails, directory_open() should return NULL.
//     if(directory_entry == NULL){
//         return NULL;
//     }

//     // malloc() space for a new struct directory.
//     struct directory *directory_stuff;
//     malloc(sizeof(*directory_stuff)); //how much space?

//     // In the struct, set the inode pointer to point to the inode returned by iget().
//     directory_stuff->inode = directory_inode;

//     // Initialize offset to 0.
//     directory_stuff->offset = 0;

//     // Return the pointer to the struct.
//     return directory_stuff;


// }

// int directory_get(struct directory *dir, struct directory_entry *ent){

//     //1. Check the offset against the size of the directory. 
//     //If the offset is greater-than or equal-to the directory size (in its inode), 
//     //we must be off the end of the directory. If so, return -1 to indicate that.

//     if(dir->offset >= dir->inode->size){ //out of bounds
//         return -1;
//     }

//     //2.   Compute the block in the directory we need to read. 
//     //The directory file itself might span multiple data blocks if there are enough entries in it. 
//     //Remember that a block only holds 128 entries. (When we just create it, it will only be one block, 
//     //but we might as well do this math now so it will work later.)

//     int data_block_index = dir->offset / 4096;

//     //3. We need to read the appropriate data block in so we can extract the directory entry from it.

//     int data_block_num = dir->inode->block_ptr[data_block_index];

//     unsigned char block[BLOCK_SIZE];
//     // and finally...
//     bread(data_block_num, block);


//     //4. Compute the offset of the directory entry in the block we just read.
//     int offset_in_block = dir->offset % 4096; //how far we are inside our block to look

//     //5. Extract the directory entry from the raw data in the block 
//     //into the struct directory_entry * we passed in.

//     //Now we have the data in our block array, 
//     //and we've computed where it starts with offset_in_block. 
//     //We just have to deserialize it.
//     //Use read_16() to extract the inode number and store it in ent->inode_num.
//     //Use strcpy() to extract the file name and store it in ent->name. 
//     //You might have to do some casting to char *.

//     //we know block is 32, we know our dir entry is at block + offset in block, use
//     //pointer arithmetic to grab name and inode num

//     //does this have a read_16()?
//     unsigned char *dir_inode = block + offset_in_block + 0; //inode
//     unsigned char *dir_name = block + offset_in_block + 2; //name
//     ent->inode_num = read_16(dir_inode);
//     strcpy(ent->name, (char *) dir_name);
// }

// void directory_close(struct directory *d){
//     //It only has to do two things:
//     //iput() the directory's in-core inode to free it up.
//     //free() the struct directory.

//     iput(d->inode);
//     free(d);
// }



// //Project 9:


// //Example usage:
// // struct inode *in = namei("/foo/bar/baz");

// struct inode *namei(char *path){
// //     The namei() function does a variety of things:
//     //if strcmp() == 0
//     if(path == "/"){
//         struct inode *root_inode = iget(ROOT_INODE_NUM);
//         return root_inode; //is this right?
//     }

//     // if((*path[0] == "/") &&(*path != "/")){
//     //     char *basename = get_basename(path);
//     //     //how to find basename inode?
//     //     //loop through icore checking inode->name?
//     //     return basename;
//     // }

//     return NULL;

// //     If the path is /, it returns the root directory's in-core inode.
// //     If the path is /foo, it returns foo's in-core inode.
// //     If the path is /foo/bar, it returns bar's in-core inode.
// //     If the path is invalid (i.e. a component isn't found), it returns NULL.

// // For this first part, simply implement namei() so it just returns the in-core inode for the root directory,

// // /. The other bits can wait until later. //

// // You'll want to use iget() with the root directory's inode number to get this information.
// // (It's OK to #define ROOT_INODE_NUM 0 and use that.)

// }


// // Create directory foo out of the root
// //int inode_num = directory_make("/foo");
// int directory_make(char *path){

//     // Find the directory path that will contain the new directory. 
//     //This should be every component of the path except the last one. 
//     //In this case, the parent directory path will be /
//     // because that's where what going to create the new directory within.

//     // Find the new directory name from the path. To do these first two steps, 
//     //use helper functions in dirbasename.c

//     //     Find the inode for the parent directory that will hold the new entry (namei()).
//     //might need to allocate size for string parent
//     char *parent = get_dirname(path);
//     struct inode *parent_inode = namei(parent); //find root parent directory of path?
 

//     //     Create a new inode for the new directory (ialloc()).
//     struct inode *new_inode = ialloc();

//     //     Create a new data block for the new directory entries (alloc()).
//     int new_block_num = alloc();

//     //     Create a new block-sized array for the new directory data block
//     unsigned char block[BLOCK_SIZE];
 
//     //      and initialize it . and .. files. The . should contain the new directory's inode number. 
//     //      The .. should contain the parent directory's inode number. 
//     //      This is like we did for the root directory in the previous project.

//     int parent_inode_num = parent_inode->inode_num; //we know the parent inode.

//     int new_inode_num = *new_inode->inode_num; //our new inode is empty, how do we know the inode num for it?

//     //how do we know what our cur parent block is? Is it always the next block in memory? That can't be?

//     unsigned char *current_dir = block +DIR_ENTRY_SIZE * 0;
//     unsigned char *parent_dir = block + DIR_ENTRY_SIZE * 1;


//     write_16(current_dir, new_inode_num); //. current //
    
//     write_16(parent_dir, parent_inode_num); //.. parent

//     unsigned char *current_name = current_dir + 2;
//     unsigned char *parent_name = parent_dir+2;

//     strcpy((char *) current_name, ".");
//     strcpy((char *) parent_name, "..");

//     //     Initialize the new directory in-core inode with a proper size and other fields, 
//     //similar to how we did with the hard-coded root directory in the previous project.

//     new_inode->flags = 2; //2 flags indicate directories

//     //     size needs to be set to the byte size of the directory. 
//     //Since we have two entries (. and ..) and each is 32 bytes, the size must be 64 bytes.
//     new_inode->size =64;//#directory entry size * 2 //IS THIS RIGHT?

//     //     block_ptr[0] needs to point to the data block we just got from alloc(), above.
//     new_inode->block_ptr[0] = new_block_num;

//     //     Write the new directory data block to disk (bwrite()).
//     bwrite(new_block_num, block);

//     //     From the parent directory inode, find the block that will contain the new directory entry 
//     //(using the size and block_ptr fields).
//     //????
//     int new_block_index = parent_inode->size / 4096;

//     //     Read that block into memory unless you're creating a new one (bread()), 
//     //and add the new directory entry to it.
//     bread(new_block_index, block);

//     //     Write that block to disk (bwrite()).
//     bwrite(new_block_index, block); 

//     //     Update the parent directory's size field (which should increase by 32, 
//     //the size of the new directory entry.

//     //     Release the new directory's in-core inode (iput()).
//     iput(parent);

//     //     Release the parent directory's in-core inode (iput()).
//     iput(new_inode);

//     // At this point, you should be able to create new directories in the root directory, e.g.
// }



// // What aret he steps?

// // Parse the directory entry
// //  split the new directory name from the rest of the path
// //  if we had a real namei, we'd look up the inode number of the path to the file
// //  our stubbed namei will return the inode number of root
// //allocate new inode for "foo"(ialloc())
// //allocate new data block for foo's entries (alloc()) 
// //in the existing directory, store the new directories inode number and name
// //'.' and '..' added to the new directory
// //write it all back to disk (bwrite(), iput())

// //bar/baz/foo.txt

// //checksums:
//     //error detection mechanism
    //feed data into a black box, and the box produces a fixed-length number representing that data.
    //if I give you the data and the checksum,
    //you recompute the checksum from the data using the same algorithm
    //you make sure the checksums match

    //related to hashes: MD5,SHA1, SHA256

    //common checksum: cyclic redundancy check(CRC, CRC-32 )

    //collision: when two files have the same checksum

    //simple algorithm: (sum of numbers) mod 10

    //1+2+3) %10 == 6

    //I send: 1, 2, 3, 6

    //you recieve: 1, 2, 3, 6 //Do the same calculation, it matches, hooray

