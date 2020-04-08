OUT:=bin/libmm
SRC:=$(shell find ./src -type f -a -name "*.cpp")
OBJ:=$(SRC:%.cpp=%.o)
CC:=g++
CFLAGS:=-nodefaultlibs -std=c++17 -o3 -I src/include -c
LFLAGS:=-nodefaultlibs -lc

ifeq ($(MAKECMDGOALS),debug)
CFLAGS+= -DDEBUG 
else ifeq ($(MAKECMDGOALS),force-debug)
CFLAGS+= -DDEBUG 
endif

$(OUT): $(OBJ)
	$(CC) -o $(OUT) $(OBJ) $(LFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: all debug clean force force-debug

all: $(OUT)
debug: all
clean:
	if [ -e "$(OUT)" ]; then rm -f "$(OUT)"; fi
	find ./src -type f -a -name "*.o" -a -exec rm '{}' \;

force: clean all
force-debug: clean debug
