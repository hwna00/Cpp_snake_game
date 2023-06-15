all:main

main:main.cpp view/Board.hpp controller/GameManager.hpp model/Snake.hpp
	g++ main.cpp -lncurses -o main