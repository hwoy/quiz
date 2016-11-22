CXX=g++
CXXFLAGS=-g -std=c++11 -pedantic -Wall

FORMAT=clang-format
FORMATFLAGS=-style=WebKit -i

BIN=quiz

.PHONY: all clean format

all: $(BIN)

$(BIN): main.o quiz.o
	$(CXX) -o $(BIN) main.o quiz.o

clean:
	rm -rf *.o *.exe $(BIN)
	
format:
	$(FORMAT) $(FORMATFLAGS) *.cpp *.hpp

main.o: main.cpp quiz.hpp
quiz.o: quiz.cpp quiz.hpp
