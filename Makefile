CC = g++
FLAGS = -Wall -Wextra -std=c++14
LINKS = -lncurses
SRCS = $(wildcard src/*.cc)
OUT = orob

default: all

all: $(SRCS)
	$(CC) $(FLAGS) -o $(OUT) $(SRCS) $(LINKS)

debug: $(SRCS)
	$(CC) -DDEBUG $(FLAGS) -o $(OUT) $(SRCS) $(LINKS)
	
clean:
	rm $(OUT) scores
