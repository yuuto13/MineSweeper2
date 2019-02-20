#ifndef MINEMAP_H
#define MINEMAP_H

#include <QRandomGenerator>

const int MIN_COL = 9;
const int MIN_ROW = 9;
const int MAX_COL = 30;
const int MAX_ROW = 24;
const int MIN_MINE_NUM = 10;
inline int MAX_MINE_NUM(int col, int row){return (col - 1) * (row - 1);}
const int x_pad[9] = {-1, 0, 1, -1, 1, -1, 0, 1, 0};
const int y_pad[9] = {-1, -1, -1, 0, 0, 1, 1, 1, 0};

class mineMap
{
public:
    mineMap();
    mineMap(int col, int row, int num);
    void setMineMap(int col, int row, int num);
    void initMap();
    void initMine(int col, int row);
    int detectMine(int col, int row);

    bool isMine(int col, int row);
    int getCol();
    int getRow();
    int getNum();

private:
    int mine_col;
    int mine_row;
    int mine_num;

    char mine_map[MAX_COL + 2][MAX_ROW + 2];
};

#endif // MINEMAP_H
