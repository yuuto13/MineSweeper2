#ifndef MYSCENE_H
#define MYSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include "myitem.h"
#include "minemap.h"
#include <QTimer>

const int GRID = 16;
const int BAR = 33;
const int GAP6 = 6;
const int GAP5 = 5;
const int GAP4 = 4;
const int GAP3 = 3;
const int GAP2 = 2;
const int Field_y = GAP3*2 + GAP6*2 + GAP2*2 + BAR;
const QString Cheat = "asd";
const int cheat_time = 3;

class myScene : public QGraphicsScene
{
    Q_OBJECT

public:
    myScene();
    myScene(int col, int row, int num);
    ~myScene();
    void setGrid(int col, int row, int num);
    void initGrid(int col, int row);
    int detectMark(int col, int row);
    void openMine(int col, int row);
    void restartMine();
    void endMine(bool boom);
    void updateMineNum();
    void setQmark(bool flag);
    int getTime();

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

signals:
    bool endGame(bool);

public slots:
    void updateTimerNum();

private:
    mineMap * mine;
    QTimer * qtimer;
    myItem * face;
    myItem * number[6];
    myItem * window[7];
    myItem * grid[MAX_COL + 2][MAX_ROW + 2];

    QString face_name;

    char key[2];
    int input_count;
    int cheat_times;
    int cursor_col, cursor_row;
    int pre_col, pre_row;
    int curr_op_num;
    int curr_mine_num;
    int curr_field_num;
    int curr_second_num;
    bool game_on;
    bool qmark;
    bool cheat_on;
};

#endif // MYSCENE_H
