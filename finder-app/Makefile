# Makefile for building the writer application

# Define the target executable
TARGET = writer

# Define the source files
SRCS = $(wildcard *.c)

# Define the object files
OBJS = $(SRCS:.c=.o)

# Define the compiler and compiler flags
CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Wextra -Werror -O2

# Default target to build the writer application
.PHONY: all
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to build the object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target to remove the writer application and object files
.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)

# Phony target to force rebuild
.PHONY: rebuild
rebuild: clean all
