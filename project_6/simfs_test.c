#include <stdio.h>
#include <string.h>
#include "ctest.h"

#include "image.h"
#include "block.h"
#include "inode.h"
#include "free.h"
#include "mkfs.h"
#include "pack.h"


//test
//include all other stuff?
//when compile it, pass in CTEST_ENABLE to TRUE in command line. Check docs


//create dummy blocks of data for them, use other functions.
//#ifdef CTEST_ENABLE

//for all tests, image_open("test image", 0)
// data_to_read[BLOCK_SIZE] = {0}; //sets all data to 0
// unsigned char 
// memcmp //compares two data things

    //create array, fill arr with anything

//
//FINISHED
void test_image(void){

    //doing reduced # of tests due to strange open errors when doing multiple open stateements

    //image_open(filename, truncate)
    //sucess: image_fd file made without problems.
    CTEST_ASSERT((image_open("image",1) != -1), "sucessful image open");


    //failure: image_fd file cannot be made.
    //CTEST_ASSERT((image_open("image", 0) == -1), "image failed to open");

    //image_close()
    //sucess: image_fd closed
    CTEST_ASSERT((image_close()  == 0), "sucessful image close");
    //failed: image_fd cannot close because its already close
    CTEST_ASSERT((image_close() == -1), "image failed to close");
}

//FINISHED
void test_block(void){

    //printf("before open\n");
    image_open("image", 1);
    //printf("after open\n");


    unsigned char full_block[BLOCK_SIZE] = {0}; //how to make this a full block? SEE MEMSET
    unsigned char empty_block[BLOCK_SIZE] = {0};
    //printf("before alloc\n");
    int block_num = alloc();
    printf("%d block num\n", block_num);

    memset(full_block, 255, BLOCK_SIZE);
    //byte with 8 bits
    //1 is just 00000001
    //256 is one full byte


    //testing alloc was sucessful
    //printf("before assert\n");
    //printf("%d\n", block_num);
    //CTEST_ASSERT((block_num != -1), "sucessful alloc");
    //printf("%d", block_num);
    //int block_num = alloc();

    bwrite(block_num, full_block);

    //test sucess: block sucessfully read from disk onto "block"
    //printf("before second assert\n");
    bread(block_num, empty_block);

    CTEST_ASSERT((memcmp(full_block, empty_block, BLOCK_SIZE) == 0), "sucessful bread and bwrite");


    //test alloc failure conditions....?
    //test failure conditions: block sucessfully failed to read from disk 
    //CTEST_ASSERT((bread())); //What failure to test?

    //alloc();
    //test sucess: Alloc sucessfully returns a free block
    CTEST_ASSERT((alloc() != -1), "sucessful alloc");

    //test failure: alloc sucessfully fails to return a free block when there are no free blocks
    unsigned char full_block_2[BLOCK_SIZE] = {0};
    memset(full_block_2, 255, BLOCK_SIZE);
    bwrite(2, full_block_2);
    CTEST_ASSERT((alloc() == -1), "failed alloc");


    image_close();
}

//FINISHED
void test_free(void){

    image_open("image", 1);

    unsigned char block[BLOCK_SIZE] = {0};
    //void set_free(unsigned char *block, int num, int set)
    //sucessfully change first bit to 1
    set_free(block, 0, 1);
    CTEST_ASSERT((find_low_clear_bit(block[0]) == 1), "testing sucessful set_bit");
    //sucessfully free first bit to 0
    set_free(block, 0, 0);
    CTEST_ASSERT((find_low_clear_bit(block[0]) == 0), "testing sucessful set_bit");

    //int find_free(unsigned char *block)
    //Sucessfully find free bit when one is available
    CTEST_ASSERT((find_free(block) == find_low_clear_bit(block[0])), "testing sucessful find free");

    //Fail to find bit when none are available
    unsigned char full_block[BLOCK_SIZE] = {0};
    memset(full_block, 255, BLOCK_SIZE);
    CTEST_ASSERT((find_free(full_block) == -1), "fails to find bit when none are available");
    //COME BACK TO THIS

    image_close();
}

//SEMI-FINISHED // How to fill up incore nodes? // Read and write wonky // check if ialloc is right?
void test_inode(void){
    //printf("inode\n");
    image_open("image", 1);


    //ialloc()

    //success: returns inode
    CTEST_ASSERT((ialloc() != NULL || -1), "sucessful ialloc");
    //failure 1: fails to find free bit
    fill_incore();

    //How to fill up every block in inode map?
    unsigned char full_block[BLOCK_SIZE] = {0};
    memset(full_block, 255, BLOCK_SIZE);
    bwrite(1, full_block);
    CTEST_ASSERT((ialloc() == NULL), "failed ialloc");

    //failure 2: fails to find incore inode because no incore spaces are free
    CTEST_ASSERT((ialloc() == NULL), "failed ialloc");
    reset_incore();
    //How to fill up incore nodes?

    //struct inode *find_incore_free(void);
    //success: finds and returns free inode

    struct inode *test_free = find_incore_free();
    //printf("%d\n", test_free->inode_num);
    CTEST_ASSERT((test_free != NULL), "sucessful find_incore_free");
    //printf("%d\n", test_free->ref_count);
    
    //fail: incore is full, returns NULL;
    fill_incore();
    CTEST_ASSERT((find_incore_free() == NULL), "failed find_incore_free");
    //printf("test\n");
    reset_incore();
    //struct inode *find_incore(unsigned int inode_num)
    //success: Finds incore that matches inode_num

    unsigned char empty_block[BLOCK_SIZE] = {0};
    //memset(empty_block, 255, BLOCK_SIZE);
    bwrite(1, empty_block);

    struct inode *setting_inode = ialloc();
    struct inode *test_find = find_incore(setting_inode->inode_num);
    //printf("%d\n", test_find->inode_num);
    CTEST_ASSERT((test_find != NULL), "sucessful find_incore");

    //failure: does not find, returns NULL


    struct inode *filled_inode = ialloc();
    filled_inode->size = 1;
    filled_inode->flags = 1;
    write_inode(filled_inode);
    struct inode *empty_inode = ialloc();
    // //initialize?
    //printf("%d\n", filled_inode->inode_num);
    //printf("%d\n", empty_inode->inode_num);
    read_inode(empty_inode, filled_inode->inode_num);
    //printf("%d\n", filled_inode->flags);

    //void read_inode(struct inode *in, int inode_num)
    //void write_inode(struct inode *in)
    //Success: inode struct is filled with data from inode at inode_num;
    CTEST_ASSERT((filled_inode->flags == empty_inode->flags), "sucessful inode read and write");
    
    //struct inode *iget(int inode_num)
    //success 1: returns numbered inode from incore if it exists
    struct inode *iget_test = iget(0);
    CTEST_ASSERT((iget_test->inode_num == 0), "sucessful iget");
    //success 2: returns free incore from incore
    //failure 1: If no free incore, return null

    //void iput(struct inode *in)
    //success: write a unwritten inode to disk
    iput(iget_test);
    //failure: returns if reference is already 0




    image_close();

}

//ALMOST FINISHED// HOW TEST MKFS? How test some failure conditions?
void test_mkfs(void){
    image_open("image", 1);

    //mkfs();
    //success: initial system is set up and allocated, root directory is created

    mkfs();
    unsigned char empty_block[BLOCK_SIZE] = {0};
    bread(2, empty_block);
    int block_num = find_free(empty_block);
    CTEST_ASSERT((block_num != 0), "sucessful mkfs");

    //check to see if blocks are listed as "in use"
    //find next free block, test that.
    //assumes other functions are working correctly if free block test works.
    //CTEST_ASSERT(()); 
    //Don't forget root directory, will take up a block, good test.


    //int directory_make(char *path)
    //success: returns 0, directory correctly made.
    CTEST_ASSERT((directory_make("/foo") == 0), "successfully made new directory");

    //failure: path does not begin with /

    // struct inode *namei(char *path)
    // success: returns root inode
    CTEST_ASSERT((namei("/") != NULL),"successful namei");

    //failure: path does not equal "/" Returns null
    CTEST_ASSERT((namei("/blahblahblah") == NULL), "failed namei");

    //struct directory * directory_open(inode_num);
    //success: returns directory at specified inode_num
    CTEST_ASSERT((directory_open(0) != NULL), "successful directory open");
    struct directory * stuff = directory_open(0);

    //failure: no directory found at inode num, reaches NULL
    //Would have to find no space in incore, otherwise a new inode is allocated and passed the num;
    fill_incore();
    CTEST_ASSERT((directory_open(-1) == NULL), "failed directory open");
    reset_incore();


    //int directory_get(struct directory *dir, struct directory_entry *ent)
    //success: returns 0, directory entry is filled;
    struct directory_entry * entry_stuff = malloc(sizeof(entry_stuff));
    CTEST_ASSERT((directory_get(stuff, entry_stuff) == 0), "sucessful directory_get");
    
    stuff->offset = 20000;
    //failure: offset greater than size, returns -1
    CTEST_ASSERT((directory_get(stuff, entry_stuff) == -1), "failed directory_get");


    // void directory_close(struct directory *d)
    // success: free's directory, doesn't crash
    directory_close(stuff);
    //CTEST_ASSERT((directory_close(stuff) ))
    //how to do when failure still returns success on false inode?





    image_close();

}


int main(void){
    //printf("before alloc");
    CTEST_VERBOSE(1);

    printf("before image\n");
    test_image();
    printf("before block\n");    
    test_block();
    printf("before free\n");
    test_free();
    printf("before inode\n");
    test_inode();
    printf("Before mkfs\n");
    test_mkfs();

    CTEST_RESULTS();
    CTEST_EXIT();

}

//#endif