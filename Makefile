CC = clang
CFLAGS =-g
SRCS = lanParty.c
TARGET = lanParty

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
