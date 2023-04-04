# CC=gcc
# CCOPTS=-Wall -Wextra -Werror
# LIBS=

# SRCS=$(wildcard *.c)
# TARGETS=$(SRCS:.c=)

# .PHONY: all clean

# all: $(TARGETS)

# clean:
# 	rm -f $(TARGETS)

# %: %.c
# 	$(CC) $(CCOPTS) -o $@ $< $(LIBS)

hellothread: hellothread.c
	gcc -Wall -Wextra -o $@ $< -lpthread