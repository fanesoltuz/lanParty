CC = gcc
CFLAGS = -Wall -Wpedantic -std=c99 -g
SRCS = main.c team.c player.c match.c bst.c
OBJS = $(SRCS:.c=.o)
TARGET = lanParty

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	rm -r out/

.PHONY: all clean

