#pragma once

#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <string>

#define ROWS 32 // 스테이지 줄 수
#define COLS 61 // 스테이지 컬럼 수


class Map {
int level, poisonCnt, growthCnt;
char data[ROWS][COLS];
public:
    Map(int level = 0) : level(level), growthCnt(level), poisonCnt(level){
        char temp[120];
        std::ifstream readFile;
        std::string src = "src/stage" + std::to_string(level + 1) + ".txt";

        readFile.open(src);

        int height = 0;
        while (height < ROWS) {
            readFile.getline(temp, 120);

            for (int i = 0; i < COLS; i++) {
                data[height][i] = temp[i];
            }
            height++;
        }

        readFile.close();
        src = "src/rules.txt";
        readFile.open(src);

        height = 0;
        while (height < level+1) {
            readFile.getline(temp, 120);
            height++;
        }
        poisonCnt = temp[0] - '0';
        growthCnt = temp[1] - '0';

        // 정상적으로 데이터를 수신하였는지 log 파일 생성
        std::string path = "src/debug/stage.txt";
        std::ofstream writeFile(path);
        if (writeFile.is_open()) {
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    writeFile << data[i][j];
                }
                writeFile << "\n";
            }
            writeFile << growthCnt << poisonCnt;
        }
    }

    int getCol() {return COLS;}
    int getRow() {return ROWS;}

    int getLevel() {return level;}

    // 사과, 독사과 개수 레벨 별 제한
    int getGrowthCnt() {return growthCnt;}
    int getPoisonCnt() {return poisonCnt;}

    void setData(int row, int col, char c) {data[row][col] = c;}

    char getData(int row, int col) {return data[row][col];}
};