CC = g++
CFLAGS = -std=c++20 -pedantic-errors -Wall -Wextra -g
PICFLAG = -fPIC
LDFLAGS = -shared
DBGFLAG = -DNDBUG

DIR = /home/user/github/modern_cpp
OBJ = $(DIR)/obj
SRC = $(DIR)/src
HDR = $(DIR)/include
TEST = $(DIR)/test

SRCS = $(wildcard $(SRC)/*.cpp)
OBJS = $(wildcard $(OBJ)/*.o)
TESTS = $(wildcard $(TEST)/*.cpp)
LIB_NAME = proj

TARGET_LIB = libproj.so
TARGET = type_traits

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
