CC = g++
CFLAGS = -std=c++11 -pedantic-errors -Wall -Wextra -g
PICFLAG = -fPIC
LDFLAGS = -shared
DBGFLAG = -DNDBUG

DIR = /home/user/github/design_patterns
OBJ = $(DIR)/obj
SRC = $(DIR)/src
HDR = $(DIR)/include
TEST = $(DIR)/test

SRCS = $(wildcard $(SRC)/*.cpp)
OBJS = $(wildcard $(OBJ)/*.o)
TESTS = $(wildcard $(TEST)/*.cpp)
LIB_NAME = despat

TARGET_LIB = libdespat.so
TARGET = logger

.PHONY: all
.PHONY: debug
.PHONY: release

all: $(TARGET)

$(TARGET): $(TARGET_LIB) $(TEST)/$(TARGET)_test.cpp
	$(CC) $(CFLAGS) -L$(DIR) -Wl,-rpath=$(DIR) \
	-I$(HDR)/ $(TEST)/$(TARGET)_test.cpp -l$(LIB_NAME) -lm -ldl \
	-pthread -o $@

$(TARGET_LIB): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(SRCS:.cpp=.o):%.o:%.cpp
	$(CC) $(CFLAGS) $(PICFLAG) -c -I$(HDR)/ $< >$@
	mv *.o $(OBJ)/

include $(SRCS:.cpp=.o)

.PHONY: clean
clean:
	rm $(SRC)/*.o
	rm $(OBJ)/*.o

.PHONY: vlg
vlg:
	valgrind --leak-check=yes --track-origins=yes ./$(TARGET)

.PHONY: tui
tui:
	gdb -tui ./$(TARGET)
