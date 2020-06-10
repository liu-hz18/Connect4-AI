all: main

CC = g++
CFLAGS = -O3 -march=native
STAND = -std=c++14
SRC_OBJECT = AI.cpp Strategy.cpp utils.cpp
H_OBJECT = AI.h Node.h Point.h Strategy.h Timer.h utils.h NodePool.h
main: $(SRC_OBJECT) $(H_OBJECT)
	$(CC) $(STAND) sdk/main.cpp sdk/json/jsoncpp.cpp $(SRC_OBJECT) $(CFLAGS) -o main