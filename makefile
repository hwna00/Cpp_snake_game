all:main

main:main.cpp view/Board.hpp controller/GameManager.hpp
	g++ main.cpp -lncurses -o main