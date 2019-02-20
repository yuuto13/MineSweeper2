#include "minemap.h"

mineMap::mineMap()
{
    this->mine_col = MIN_COL;
    this->mine_row = MIN_ROW;
    this->mine_num = MIN_MINE_NUM;
    this->initMap();
}

mineMap::mineMap(int col, int row, int num)
{
    this->mine_col = col;
    this->mine_row = row;
    this->mine_num = num;
    this->initMap();
}

void mineMap::setMineMap(int col, int row, int num)
{
    this->mine_col = col;
    this->mine_row = row;
    this->mine_num = num;
}

void mineMap::initMap()
{
    memset(this->mine_map, '*', (MAX_COL + 2) * (MAX_ROW + 2));
    for(int i = 0; i < this->mine_row; i++){
        for(int j = 0; j < this->mine_col; j++){
            this->mine_map[j + 1][i + 1] = '0';
        }
    }
}

void mineMap::initMine(int col, int row)
{
    int rand_col, rand_row;
    int mine_count = mine_num;
    if(mine_num < mine_col * mine_row / 2){
        while(mine_count){
            rand_col = QRandomGenerator::global()->generate() % mine_col + 1;
            rand_row = QRandomGenerator::global()->generate() % mine_row + 1;
            if(mine_map[rand_col][rand_row] == '0' && rand_col != col && rand_row != row){
                mine_map[rand_col][rand_row] = '1';
                mine_count--;
            }
        }
    }
    else{
        for(int i = 0; i < mine_row; i++){
            for(int j = 0; j < mine_col; j++){
                if(i + 1 != row && j + 1 != col){
                    mine_map[j + 1][i + 1] = '1';
                }
            }
        }
        while(mine_col * mine_row - mine_count - 1){
            rand_col = QRandomGenerator::global()->generate() % mine_col + 1;
            rand_row = QRandomGenerator::global()->generate() % mine_row + 1;
            if(mine_map[rand_col][rand_row] == '1'){
                mine_map[rand_col][rand_row] = '0';
                mine_count++;
            }
        }
    }
}

int mineMap::detectMine(int col, int row)
{
    int mine_count = 0;
    for(int i = 0; i < 8; i++){
        if (mine_map[col + x_pad[i]][row + y_pad[i]] == '1'){
            mine_count++;
        }
    }
    return mine_count;
}


bool mineMap::isMine(int col, int row)
{
    if(this->mine_map[col][row] == '1'){
        return true;
    }
    else return false;
}

int mineMap::getCol()
{
    return this->mine_col;
}

int mineMap::getRow()
{
    return this->mine_row;
}

int mineMap::getNum()
{
    return this->mine_num;
}
