CC = gcc
CFLAGS = -g
SRCS = main.c team.c player.c
OBJS = $(SRCS:.c=.o)
TARGET = lanParty

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean

