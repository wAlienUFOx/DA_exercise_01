
CXX = g++
CXXFLAGS = -Wstrict-overflow=0 -std=c++11 -O2 -Wextra -Wall -Werror -Wno-sign-compare -Wno-unused-result -pedantic -O0 -g
FILES = main.cpp
NAME = solution

all: main

main:
	$(CXX) $(CXXFLAGS) -o $(NAME) $(FILES)

clean:
	rm -f *.o $(NAME)
