simfs_test: simfs_test.o simfs.a
	gcc -Wall -Wextra -o $@ $^

simfs_test.o: simfs_test.c
	gcc -Wall -Wextra -c $< 

simfs.a: image.o block.o inode.o free.o mkfs.o pack.o dirbasename.o ls.o
	ar rcs $@ $^

image.o: image.c
	gcc -Wall -Wextra -c $<

block.o: block.c
	gcc -Wall -Wextra -c $<

free.o: free.c
	gcc -Wall -Wextra -c $<

inode.o: inode.c
	gcc -Wall -Wextra -c $<

# mkfs: mkfs.o simfs.a
# 	gcc -Wall -Wextra -o $@ $^ 

mkfs.o: mkfs.c
	gcc -Wall -Wextra -c $<

pack.o: pack.c
	gcc -Wall -Wextra -c $<

dirbasename.o: dirbasename.c
	gcc -Wall -Wextra -c $<

ls.o: ls.c
	gcc -Wall -Wextra -c $<

.PHONY: test

test: simfs_test
	./simfs_test

clean: 
	rm -f *.o