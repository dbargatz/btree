SRCS = src/btree.c src/node.c test/main.c
OBJS = $(SRCS:.c=.o)
BINARY = btree
CFLAGS += -Wall -Werror

.PHONY: all clean

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(BINARY) $(OBJS)

clean:
	@rm -rf $(OBJS) $(BINARY)
