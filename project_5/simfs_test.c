#include <stdio.h>
#include <string.h>
#include "ctest.h"

#include "image.h"
#include "block.h"
#include "inode.h"
#include "free.h"
#include "mkfs.h"


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
    //printf("%d block num\n", block_num);

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
void test_free(void){

    image_open("image", 1);

    //DONT ACTUALLY NEED THIS TEST, PROVIDED BY BEEJ
    //finds clear bit
    //Sucess: returns correct free bit
    // unsigned char x = 00000111;
    // unsigned char full_byte = 11111111; //Not correct?
    // int full = find_low_clear_bit(full_byte);
    // int test = find_low_clear_bit(x);
    // printf("test is %d, full is %d\n", test, full);
    // CTEST_ASSERT((find_low_clear_bit(x) == 5), "Sucessful find_Free");
    // //sucessful failure: returns no free bit when full
    // CTEST_ASSERT((find_low_clear_bit(full_byte) == -1), "No free bit for find_Free");


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

    image_close();

}

//finished
void test_inode(void){
    //printf("inode\n");
    image_open("image", 1);


    //unsigned char full_block[BLOCK_SIZE] = {1}; //how to make this a full block?
    //unsigned char empty_block[BLOCK_SIZE] = {0};

    //test sucess: returns a free node
    //printf("before ialloc()\n");
    //alloc();
    //test sucess: Alloc sucessfully returns a free block
    CTEST_ASSERT((ialloc() != -1), "sucessful alloc");

    //test failure: alloc sucessfully fails to return a free block when there are no free blocks
    unsigned char full_block_2[BLOCK_SIZE] = {0};
    memset(full_block_2, 255, BLOCK_SIZE);
    bwrite(1, full_block_2);
    CTEST_ASSERT((ialloc() == -1), "failed ialloc");


    image_close();

}

//finished
void test_mkfs(void){
    image_open("image", 1);

    //mkfs();
    //success: initial system is set up and allocated, root directory is created
    mkfs();
    unsigned char empty_block[BLOCK_SIZE] = {0};
    bread(2, empty_block);
    int block_num = find_free(empty_block);
    printf("%d\n", block_num);
    CTEST_ASSERT((block_num != 0), "sucessful mkfs");


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