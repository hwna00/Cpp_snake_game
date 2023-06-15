all:main

main:main.cpp view/* controller/* model/*
	g++ main.cpp -lncurses -o main