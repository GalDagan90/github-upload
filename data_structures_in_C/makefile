CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -pthread -g 
PICFLAG = -fPIC
LDFLAGS = -shared
DBGFLAG = -DNDBUG

OBJ = obj
SRC = src
HDR = include
TEST = test
DIR = /home/user/git/ds

SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(wildcard $(OBJ)/*.o)
TESTS = $(wildcard $(TEST)/*.c)
LIB_NAME = ds

TARGET_LIB = libds.so
TARGET = watchdog

.PHONY: all
.PHONY: debug
.PHONY: release

all: $(TARGET)

$(TARGET): $(TARGET_LIB) $(TEST)/$(TARGET)_test.c
	$(CC) $(CFLAGS) -L$(DIR) -Wl,-rpath=$(DIR) \
	-I$(HDR)/ $(TEST)/$(TARGET)_test.c -l$(LIB_NAME) -lm -o $@

$(TARGET_LIB): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(SRCS:.c=.o):%.o:%.c
	$(CC) $(CFLAGS) $(PICFLAG) -c -I$(HDR)/ $< >$@
	mv *.o $(OBJ)/
		
include $(SRCS:.c=.o)

.PHONY: clean
clean:
	rm $(SRC)/*.o 

.PHONY: vlg
vlg:
	valgrind --leak-check=yes --track-origins=yes ./$(TARGET)

.PHONY: tui
tui:
	gdb -tui ./$(TARGET)