


//useful subfunction that finds first clear 0 bit, -1 if no clear bits. 0th bit is rightmost bit.
int find_low_clear_bit(unsigned char x)
{
    for (int i = 0; i < 8; i++)
        if (!(x & (1 << i)))
            return i;
    
    return -1;
}


//set a specific bit to the value in set (0 or 1)
void set_free(unsigned char *block, int num, int set){

    int byte_num = num / 8;  //# 8 bits per byte
    int bit_num = num % 8;

    if (set == 0){
        //block[byte_num] |= set << bit_num;
        block[byte_num] = block[byte_num] & ~(1 << bit_num);
    }
    else{
        block[byte_num] |= set << bit_num;
    }



}
// find a 0 bit and return its index (i.e. the block number that corresponds to this bit.
int find_free(unsigned char *block){

    //create loop going through every byte in block?
    int free;
    int mathy = 0;

    for (int i = 0; i < 4096; i++){
        //check if byte is free
        free = find_low_clear_bit(block[i]);
        if (free != -1){
            mathy = i * 8;
            return mathy + free;
        }
           
    }

    return -1;

}