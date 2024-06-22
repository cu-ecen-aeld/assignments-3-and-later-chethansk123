CC := gcc
CFLAGS := -Wall -Wextra -pedantic -std=c99 -D_POSIX_C_SOURCE=200809L
TARGET := aesdsocket
SRCS := aesdsocket.c
OBJS := $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean

