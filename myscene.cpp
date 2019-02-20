#include "myscene.h"

myScene::myScene(int col, int row, int num)
{
    this->mine = new mineMap(col, row, num);
    this->qtimer = new QTimer;
    connect(this->qtimer, SIGNAL(timeout()), this, SLOT(updateTimerNum()));

    key[0] = 0, key[1] = 0;
    input_count = 0;
    cheat_times = 0;
    cursor_col = 0, cursor_row = 0;
    pre_col = 0, pre_row = 0;
    curr_op_num = 0;
    curr_mine_num = this->mine->getNum();
    curr_field_num = this->mine->getCol() * this->mine->getRow();
    curr_second_num = 0;
    game_on = true;
    cheat_on = false;

    this->window[0] = new myItem("window", "back_ground");
    this->window[1] = new myItem("window", "field_bottom_corner");
    this->window[2] = new myItem("window", "field_top_corner");
    this->window[3] = new myItem("window", "number_display");
    this->window[4] = new myItem("window", "number_display");
    this->window[5] = new myItem("window", "status_bar_bottom_corner");
    this->window[6] = new myItem("window", "status_bar_top_corner");
    this->window[0]->setRect(0, 0, GAP3*3 + GAP6 + GAP5 + GRID * this->mine->getCol(), Field_y + GAP3 + GAP5 + GRID * this->mine->getRow());
    this->window[1]->setRect(0, GRID * (MAX_ROW - this->mine->getRow()), GRID * this->mine->getCol() + GAP3*2, 387);
    this->window[2]->setRect(GRID * (MAX_COL - this->mine->getCol()), 0, 486, GRID * this->mine->getRow() + GAP3);
    this->window[5]->setRect(0, 0, GRID * this->mine->getCol() + GAP3*2, 35);
    this->window[6]->setRect(GRID * (MAX_COL - this->mine->getCol()), 0, 486, 35);
    this->window[0]->setPos(GAP3, GAP3);
    this->window[1]->setPos(GAP3 + GAP6, Field_y);
    this->window[2]->setPos(GAP3 + GAP6, Field_y - GAP3);
    this->window[3]->setPos(GAP3 + GAP6 + GAP2 + GAP5, GAP3 + GAP6 + GAP2 + GAP4);
    this->window[4]->setPos(GAP3*3 + GAP6 + GRID * this->mine->getCol() - GAP2 - GAP5 - 41, GAP3 + GAP6 + GAP2 + GAP4);
    this->window[5]->setPos(GAP3 + GAP6, GAP3 + GAP6 + GAP2);
    this->window[6]->setPos(GAP3 + GAP6, GAP3 + GAP6);
    this->addItem(this->window[0]);
    this->addItem(this->window[1]);
    this->addItem(this->window[2]);
    this->addItem(this->window[3]);
    this->addItem(this->window[4]);
    this->addItem(this->window[5]);
    this->addItem(this->window[6]);
    QString str_num = QString("%1").arg(curr_mine_num, 3, 10, QChar('0'));
    this->number[0] = new myItem("number", str_num.mid(0, 1));
    this->number[1] = new myItem("number", str_num.mid(1, 1));
    this->number[2] = new myItem("number", str_num.mid(2, 1));
    this->number[3] = new myItem("number", "0");
    this->number[4] = new myItem("number", "0");
    this->number[5] = new myItem("number", "0");
    this->number[0]->setPos(GAP3 + GAP6 + GAP2*2 + GAP5, GAP3 + GAP6 + GAP2*2 + GAP4);
    this->number[1]->setPos(GAP3 + GAP6 + GAP2*3 + GAP5 + 11, GAP3 + GAP6 + GAP2*2 + GAP4);
    this->number[2]->setPos(GAP3 + GAP6 + GAP2*4 + GAP5 + 11*2, GAP3 + GAP6 + GAP2*2 + GAP4);
    this->number[3]->setPos(GAP3*3 + GAP6 + GRID * this->mine->getCol() - GAP5 - 41, GAP3 + GAP6 + GAP2*2 + GAP4);
    this->number[4]->setPos(GAP3*3 + GAP6 + GAP2 + GRID * this->mine->getCol() - GAP5 - 41 + 11, GAP3 + GAP6 + GAP2*2 + GAP4);
    this->number[5]->setPos(GAP3*3 + GAP6 + GAP2*2 + GRID * this->mine->getCol() - GAP5 - 41 + 11*2, GAP3 + GAP6 + GAP2*2 + GAP4);
    this->addItem(this->number[0]);
    this->addItem(this->number[1]);
    this->addItem(this->number[2]);
    this->addItem(this->number[3]);
    this->addItem(this->number[4]);
    this->addItem(this->number[5]);
    this->face = new myItem("face", "smile");
    this->face->setPos(GAP3*2 + GAP6 + GRID/2 * this->mine->getCol() - 26/2, GAP3*2 + GAP6 + GAP2);
    this->addItem(this->face);
    this->initGrid(this->mine->getCol(), this->mine->getRow());
}

myScene::~myScene()
{
    delete mine;
    delete qtimer;
    delete face;
    int i = 0, j = 0;
    for(i = 0; i < 6; i++){
        delete number[i];
    }
    for(i = 0; i < 7; i++){
        delete window[i];
    }
    for(i = 0; i < MAX_COL + 2; i++){
        for(j = 0; j < MAX_ROW + 2; j++){
            delete grid[i][j];
        }
    }
}

void myScene::setGrid(int col, int row, int num)
{
    this->qtimer->stop();
    int i, j;
    for(i = 1; i < this->mine->getCol() + 1; i++){
        for(j = 1; j < this->mine->getRow() + 1; j++){
            this->removeItem(this->grid[i][j]);
        }
    }
    this->mine->setMineMap(col, row, num);
    key[0] = 0, key[1] = 0;
    input_count = 0;
    cheat_times = 0;
    curr_op_num = 0;
    curr_mine_num = this->mine->getNum();
    curr_field_num = this->mine->getCol() * this->mine->getRow();
    curr_second_num = 0;
    this->window[0]->setPic("window", "back_ground");
    this->window[1]->setPic("window", "field_bottom_corner");
    this->window[2]->setPic("window", "field_top_corner");
    this->window[3]->setPic("window", "number_display");
    this->window[4]->setPic("window", "number_display");
    this->window[5]->setPic("window", "status_bar_bottom_corner");
    this->window[6]->setPic("window", "status_bar_top_corner");
    this->window[0]->setRect(0, 0, GAP3*3 + GAP6 + GAP5 + GRID * this->mine->getCol(), Field_y + GAP3 + GAP5 + GRID * this->mine->getRow());
    this->window[1]->setRect(0, GRID * (MAX_ROW - this->mine->getRow()), GRID * this->mine->getCol() + GAP3*2, 387);
    this->window[2]->setRect(GRID * (MAX_COL - this->mine->getCol()), 0, 486, GRID * this->mine->getRow() + GAP3);
    this->window[5]->setRect(0, 0, GRID * this->mine->getCol() + GAP3*2, 35);
    this->window[6]->setRect(GRID * (MAX_COL - this->mine->getCol()), 0, 486, 35);
    this->window[0]->setPos(GAP3, GAP3);
    this->window[1]->setPos(GAP3 + GAP6, Field_y);
    this->window[2]->setPos(GAP3 + GAP6, Field_y - GAP3);
    this->window[3]->setPos(GAP3 + GAP6 + GAP2 + GAP5, GAP3 + GAP6 + GAP2 + GAP4);
    this->window[4]->setPos(GAP3*3 + GAP6 + GRID * this->mine->getCol() - GAP2 - GAP5 - 41, GAP3 + GAP6 + GAP2 + GAP4);
    this->window[5]->setPos(GAP3 + GAP6, GAP3 + GAP6 + GAP2);
    this->window[6]->setPos(GAP3 + GAP6, GAP3 + GAP6);
    this->updateMineNum();
    this->number[3]->setPic("number", "0");
    this->number[4]->setPic("number", "0");
    this->number[5]->setPic("number", "0");
    this->number[0]->setPos(GAP3 + GAP6 + GAP2*2 + GAP5, GAP3 + GAP6 + GAP2*2 + GAP4);
    this->number[1]->setPos(GAP3 + GAP6 + GAP2*3 + GAP5 + 11, GAP3 + GAP6 + GAP2*2 + GAP4);
    this->number[2]->setPos(GAP3 + GAP6 + GAP2*4 + GAP5 + 11*2, GAP3 + GAP6 + GAP2*2 + GAP4);
    this->number[3]->setPos(GAP3*3 + GAP6 + GRID * this->mine->getCol() - GAP5 - 41, GAP3 + GAP6 + GAP2*2 + GAP4);
    this->number[4]->setPos(GAP3*3 + GAP6 + GAP2 + GRID * this->mine->getCol() - GAP5 - 41 + 11, GAP3 + GAP6 + GAP2*2 + GAP4);
    this->number[5]->setPos(GAP3*3 + GAP6 + GAP2*2 + GRID * this->mine->getCol() - GAP5 - 41 + 11*2, GAP3 + GAP6 + GAP2*2 + GAP4);
    this->face->setPic("face", "smile");
    this->face->setPos(GAP3*2 + GAP6 + GRID/2 * this->mine->getCol() - 26/2, GAP3*2 + GAP6 + GAP2);
    for(i = 0; i < MAX_COL + 2; i++){
        for(j = 0; j < MAX_ROW + 2; j++){
            this->grid[i][j]->setPic("grid", "null");
        }
    }
    for(i = 1; i < this->mine->getCol() + 1; i++){
        for(j = 1; j < this->mine->getRow() + 1; j++){
            this->grid[i][j]->setPic("grid", "field");
            this->grid[i][j]->setPos(GAP3*2 + GAP6 + (i - 1) * GRID, Field_y + (j - 1) * GRID);
            this->addItem(this->grid[i][j]);
        }
    }
    game_on = true;
}

void myScene::initGrid(int col, int row)
{
    int i, j;
    for(i = 0; i < MAX_COL + 2; i++){
        for(j = 0; j < MAX_ROW + 2; j++){
            this->grid[i][j] = new myItem("grid", "null");
        }
    }
    for(i = 1; i < col + 1; i++){
        for(j = 1; j < row + 1; j++){
            this->grid[i][j]->setPic("grid", "field");
            this->grid[i][j]->setPos(GAP3*2 + GAP6 + (i - 1) * GRID, Field_y + (j - 1) * GRID);
            this->addItem(this->grid[i][j]);
        }
    }
}

void myScene::updateMineNum()
{
    QString str_num;
    if(curr_mine_num >= 0){
        str_num = QString("%1").arg(curr_mine_num, 3, 10, QChar('0'));
    }else{
        curr_mine_num = -curr_mine_num % 100;
        str_num = QString("-%1").arg(curr_mine_num, 2, 10, QChar('0'));
    }
    this->number[0]->setPic("number", str_num.mid(0, 1));
    this->number[1]->setPic("number", str_num.mid(1, 1));
    this->number[2]->setPic("number", str_num.mid(2, 1));
}

void myScene::updateTimerNum()
{
    curr_second_num++;
    if(curr_second_num >= 999){
        curr_second_num = 999;
        this->qtimer->stop();
    }
    QString str_num;
    str_num = QString("%1").arg(curr_second_num, 3, 10, QChar('0'));
    this->number[3]->setPic("number", str_num.mid(0, 1));
    this->number[4]->setPic("number", str_num.mid(1, 1));
    this->number[5]->setPic("number", str_num.mid(2, 1));
}

int myScene::detectMark(int col, int row)
{
    int mark_count = 0;
    for(int i = 0; i < 8; i++){
        if (this->grid[col + x_pad[i]][row + y_pad[i]]->getName() == "mark"){
            mark_count++;
        }
    }
    return mark_count;
}

void myScene::openMine(int col, int row)
{
    if(this->grid[col][row]->getName() != 'null'){
        int mine_count;
        for(int i = 0; i < 8; i++){
            mine_count = this->mine->detectMine(col + x_pad[i], row + y_pad[i]);
            if(this->grid[col + x_pad[i]][row + y_pad[i]]->getName() == "empty"
                    || this->grid[col + x_pad[i]][row + y_pad[i]]->getName() == "field"
                    || this->grid[col + x_pad[i]][row + y_pad[i]]->getName() == "qmark_pressed"){
                if(mine_count == 0){
                    this->grid[col + x_pad[i]][row + y_pad[i]]->setPic("grid", "mine_0");
                    curr_field_num--;
                    openMine(col + x_pad[i], row + y_pad[i]);
                }
                else{
                    this->grid[col + x_pad[i]][row + y_pad[i]]->setPic("grid", QString("mine_%1").arg(mine_count));
                    curr_field_num--;
                }
            }
        }
    }
}

void myScene::restartMine()
{
    this->qtimer->stop();
    this->face->setPic("face", "smile");
    this->mine->initMap();
    key[0] = 0, key[1] = 0;
    input_count = 0;
    cheat_times = 0;
    curr_op_num = 0;
    curr_mine_num = this->mine->getNum();
    curr_field_num = this->mine->getCol() * this->mine->getRow();
    curr_second_num = 0;
    this->updateMineNum();
    this->number[3]->setPic("number", "0");
    this->number[4]->setPic("number", "0");
    this->number[5]->setPic("number", "0");
    for(int i = 1; i < this->mine->getCol() + 1; i++){
        for(int j = 1; j < this->mine->getRow() + 1; j++){
            this->grid[i][j]->setPic("grid", "field");
        }
    }
    game_on = true;
}

void myScene::endMine(bool boom)
{
    this->qtimer->stop();
    game_on = false;
    emit endGame(boom);
    if(boom){
        this->face->setPic("face", "dead");
        for(int i = 1; i < this->mine->getCol() + 1; i++){
            for(int j = 1; j < this->mine->getRow() + 1; j++){
                if(this->grid[i][j]->getName() == "mark" && !this->mine->isMine(i, j)){
                    this->grid[i][j]->setPic("grid", "mine_fause");
                }
                else if(this->grid[i][j]->getName() != "mark" && this->grid[i][j]->getName() != "mine_true" && this->mine->isMine(i, j)){
                    this->grid[i][j]->setPic("grid", "mine");
                }
            }
        }
    }
    else{
        this->face->setPic("face", "cool");
        curr_mine_num = 0;
        this->updateMineNum();
        for(int i = 1; i < this->mine->getCol() + 1; i++){
            for(int j = 1; j < this->mine->getRow() + 1; j++){
                if(this->mine->isMine(i, j)){
                    this->grid[i][j]->setPic("grid", "mark");
                }
            }
        }
    }
}

void myScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == key[0]){
        key[0] = Qt::NoButton;
        if(key[1] != Qt::NoButton){
            key[0] = key[1];
            key[1] = Qt::NoButton;
        }
    }else if(event->button() == key[1]){
        key[1] = Qt::NoButton;
    }
    if(this->face->isUnderMouse()){
        if(curr_op_num != 0){
            this->restartMine();
            return;
        }
        this->face->setPic("face", "smile");
        return;
    }
    if(game_on){
        this->face->setPic("face", "smile");
        if(event->scenePos().x() < GAP3*2 + GAP6 + GRID * this->mine->getCol() && event->scenePos().x() >= GAP3*2 + GAP6
                && event->scenePos().y() < Field_y + GRID * this->mine->getRow() && event->scenePos().y() >= Field_y){
            this->cursor_col = (event->scenePos().x() - GAP3*2 - GAP6) / GRID + 1;
            this->cursor_row = (event->scenePos().y() - Field_y) / GRID + 1;
            if(key[0] == Qt::NoButton && event->button() == Qt::LeftButton){
                if(this->grid[cursor_col][cursor_row]->getName() == "empty"
                        || this->grid[cursor_col][cursor_row]->getName() == "qmark_pressed"){
                    if(curr_op_num == 0){
                        this->mine->initMine(cursor_col, cursor_row);
                        this->updateTimerNum();
                        this->qtimer->start(1000);
                    }
                    if(this->mine->isMine(cursor_col, cursor_row)){
                        this->grid[cursor_col][cursor_row]->setPic("grid", "mine_true");
                        curr_op_num++;
                        this->endMine(true);
                    }
                    else{
                        if(this->mine->detectMine(cursor_col, cursor_row) == 0){
                            this->grid[cursor_col][cursor_row]->setPic("grid", "mine_0");
                            curr_field_num--;
                            this->openMine(cursor_col, cursor_row);
                        }else{
                            this->grid[cursor_col][cursor_row]->setPic("grid", QString("mine_%1").arg(this->mine->detectMine(cursor_col, cursor_row)));
                            curr_field_num--;
                        }
                        curr_op_num++;
                        if(curr_field_num <= this->mine->getNum()){
                            this->endMine(false);
                        }
                    }
                }
            }
            else if(key[0] != Qt::NoButton){
                if(this->grid[cursor_col][cursor_row]->getName().length() == 6
                        && this->mine->detectMine(cursor_col, cursor_row) == this->detectMark(cursor_col, cursor_row)){
                    for(int i = 0; i < 8; i++){
                        if(this->mine->isMine(cursor_col + x_pad[i], cursor_row + y_pad[i])
                                && this->grid[cursor_col + x_pad[i]][cursor_row + y_pad[i]]->getName() != "mark"){
                            this->grid[cursor_col + x_pad[i]][cursor_row + y_pad[i]]->setPic("grid", "mine_true");
                            this->endMine(true);
                        }
                    }
                    this->openMine(cursor_col, cursor_row);
                    curr_op_num++;
                    if(curr_field_num <= this->mine->getNum()){
                        this->endMine(false);
                    }
                }
                else{
                    for(int i = 0; i < 9; i++){
                        if(this->grid[cursor_col + x_pad[i]][cursor_row + y_pad[i]]->getName() == "empty"){
                            this->grid[cursor_col + x_pad[i]][cursor_row + y_pad[i]]->setPic("grid", "field");
                        }
                        else if(this->grid[cursor_col + x_pad[i]][cursor_row + y_pad[i]]->getName() == "qmark_pressed"){
                            this->grid[cursor_col + x_pad[i]][cursor_row + y_pad[i]]->setPic("grid", "qmark");
                        }
                    }
                }
            }
            this->pre_col = (event->scenePos().x() - GAP3*2 - GAP6) / GRID + 1;
            this->pre_row = (event->scenePos().y() - Field_y) / GRID + 1;
        }
    }
}

void myScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(key[0] == Qt::NoButton){
        key[0] = event->button();
    }else{
        key[1] = event->button();
    }
    if(this->face->isUnderMouse() && event->button() == Qt::LeftButton){
        face_name = this->face->getName();
        this->face->setPic("face", "smile_pressed");
        return;
    }
    if(game_on){
        if(event->scenePos().x() < GAP3*2 + GAP6 + GRID * this->mine->getCol() && event->scenePos().x() >= GAP3*2 + GAP6
                && event->scenePos().y() < Field_y + GRID * this->mine->getRow() && event->scenePos().y() >= Field_y){
            this->cursor_col = (event->scenePos().x() - GAP3*2 - GAP6) / GRID + 1;
            this->cursor_row = (event->scenePos().y() - Field_y) / GRID + 1;
            if(key[1] != Qt::NoButton){
                for(int i = 0; i < 9; i++){
                    if(this->grid[cursor_col + x_pad[i]][cursor_row + y_pad[i]]->getName() == "field"){
                        this->grid[cursor_col + x_pad[i]][cursor_row + y_pad[i]]->setPic("grid", "empty");
                    }
                    else if(this->grid[cursor_col + x_pad[i]][cursor_row + y_pad[i]]->getName() == "qmark"){
                        this->grid[cursor_col + x_pad[i]][cursor_row + y_pad[i]]->setPic("grid", "qmark_pressed");
                    }
                }
            }else if(key[0] == Qt::LeftButton){
                this->face->setPic("face", "oh");
                if(this->grid[cursor_col][cursor_row]->getName() == "field"){
                    if(cheat_on){
                        if(cheat_times >= cheat_time){
                            curr_second_num += 10 * (cheat_times - cheat_time + 1);
                            this->updateTimerNum();
                        }
                        cheat_times++;
                        if(this->mine->isMine(cursor_col, cursor_row)){
                            this->grid[cursor_col][cursor_row]->setPic("grid", "mark");
                            curr_mine_num--;
                            this->updateMineNum();
                        }else{
                           this->grid[cursor_col][cursor_row]->setPic("grid", "empty");
                        }
                    }else{
                        this->grid[cursor_col][cursor_row]->setPic("grid", "empty");
                    }
                }
                else if(this->grid[cursor_col][cursor_row]->getName() == "qmark"){
                    this->grid[cursor_col][cursor_row]->setPic("grid", "qmark_pressed");
                }
            }else if(key[0] == Qt::RightButton){
                if(this->grid[cursor_col][cursor_row]->getName() == "field"){
                    this->grid[cursor_col][cursor_row]->setPic("grid", "mark");
                    curr_mine_num--;
                    this->updateMineNum();
                }else if(this->grid[cursor_col][cursor_row]->getName() == "mark"){
                    if(qmark){
                        this->grid[cursor_col][cursor_row]->setPic("grid", "qmark");
                    }else{
                        this->grid[cursor_col][cursor_row]->setPic("grid", "field");
                    }
                    curr_mine_num++;
                    this->updateMineNum();
                }else if(this->grid[cursor_col][cursor_row]->getName() == "qmark"){
                    this->grid[cursor_col][cursor_row]->setPic("grid", "field");
                }
            }
            this->pre_col = cursor_col;
            this->pre_row = cursor_row;
        }else if(!this->face->isUnderMouse() && key[0] == Qt::LeftButton){
            this->face->setPic("face", "oh");
        }
    }
}

void myScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    this->mousePressEvent(event);
}

void myScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    int i = 0;
    if(this->face->isUnderMouse()){
        if(this->face->getName() != "smile_pressed"){
            face_name = this->face->getName();
            this->face->setPic("face", "smile_pressed");
            return;
        }
    }else{
        if(this->face->getName() == "smile_pressed"){
            this->face->setPic("face", face_name);
            return;
        }
    }
    if(game_on){
        if(event->scenePos().x() < GAP3*2 + GAP6 + GRID * this->mine->getCol() && event->scenePos().x() >= GAP3*2 + GAP6
                && event->scenePos().y() < Field_y + GRID * this->mine->getRow() && event->scenePos().y() >= Field_y){
            this->cursor_col = (event->scenePos().x() - GAP3*2 - GAP6) / GRID + 1;
            this->cursor_row = (event->scenePos().y() - Field_y) / GRID + 1;
            if(cursor_col != pre_col || cursor_row != pre_row){
                if(key[0] == Qt::LeftButton){
                    if(this->grid[pre_col][pre_row]->getName() == "empty"){
                       this->grid[pre_col][pre_row]->setPic("grid", "field");
                    }
                    else if(this->grid[pre_col][pre_row]->getName() == "qmark_pressed"){
                        this->grid[pre_col][pre_row]->setPic("grid", "qmark");
                    }
                    if(this->grid[cursor_col][cursor_row]->getName() == "field"){
                        this->grid[cursor_col][cursor_row]->setPic("grid", "empty");
                    }
                    else if(this->grid[cursor_col][cursor_row]->getName() == "qmark"){
                        this->grid[cursor_col][cursor_row]->setPic("grid", "qmark_pressed");
                    }
                }
                if(key[1] != Qt::NoButton){
                    if(pre_col != 0 && pre_row != 0){
                        for(i = 0; i < 9; i++){
                            if(this->grid[pre_col + x_pad[i]][pre_row + y_pad[i]]->getName() == "empty"){
                                this->grid[pre_col + x_pad[i]][pre_row + y_pad[i]]->setPic("grid", "field");
                            }
                            else if(this->grid[pre_col + x_pad[i]][pre_row + y_pad[i]]->getName() == "qmark_pressed"){
                                this->grid[pre_col + x_pad[i]][pre_row + y_pad[i]]->setPic("grid", "qmark");
                            }
                        }
                    }
                    for(i = 0; i < 9; i++){
                        if(this->grid[cursor_col + x_pad[i]][cursor_row + y_pad[i]]->getName() == "field"){
                            this->grid[cursor_col + x_pad[i]][cursor_row + y_pad[i]]->setPic("grid", "empty");
                        }
                        if(this->grid[cursor_col + x_pad[i]][cursor_row + y_pad[i]]->getName() == "qmark"){
                            this->grid[cursor_col + x_pad[i]][cursor_row + y_pad[i]]->setPic("grid", "qmark_pressed");
                        }
                    }
                }
            }
            this->pre_col = cursor_col;
            this->pre_row = cursor_row;
        }
        else{
            if(pre_col != 0 && pre_row != 0){
                for(i = 0; i < 9; i++){
                    if(this->grid[pre_col + x_pad[i]][pre_row + y_pad[i]]->getName() == "empty"){
                        this->grid[pre_col + x_pad[i]][pre_row + y_pad[i]]->setPic("grid", "field");
                    }
                    else if(this->grid[pre_col + x_pad[i]][pre_row + y_pad[i]]->getName() == "qmark_pressed"){
                        this->grid[pre_col + x_pad[i]][pre_row + y_pad[i]]->setPic("grid", "qmark");
                    }
                }
            }
            this->cursor_col = 0;
            this->cursor_row = 0;
            this->pre_col = 0;
            this->pre_row = 0;
        }
    }
}

void myScene::keyPressEvent(QKeyEvent *event)
{
    if(event->text() == "p"){
        if(this->face->getName() == "smile"){
            if(game_on == true){
                this->qtimer->stop();
                game_on = false;
            }
            else if(game_on == false){
                this->qtimer->start();
                game_on = true;
            }
        }
    }
    if(input_count != Cheat.length()){
        if(event->text() == Cheat.data()[input_count]){
            if(input_count == Cheat.length() - 1){
                cheat_on = true;
            }
            input_count++;
        }
        else{
            input_count = 0;
        }
        if(event->text() == "c"){
            cheat_on = true;
        }
    }
}

void myScene::keyReleaseEvent(QKeyEvent *event)
{
    if(event->text() == "c" && input_count != Cheat.length()){
        cheat_on = false;
    }
}

void myScene::setQmark(bool flag)
{
    this->qmark = flag;
}

int myScene::getTime()
{
    return curr_second_num;
}
