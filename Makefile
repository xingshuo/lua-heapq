CFLAGS = -g3 -O0 -Wall
SHARED := -fPIC --shared
INC = include
SRC = src
BUILD = build

all: $(BUILD)/heapq.so

$(BUILD):
	mkdir $(BUILD)

$(BUILD)/heapq.so: $(SRC)/lua-heapq.c $(SRC)/heapq.c | $(BUILD)
	gcc $(CFLAGS) $(SHARED) $^ -o $@ -I$(INC)

run:
	bin/lua test.lua

clean:
	rm $(BUILD)/*
