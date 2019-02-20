#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->settings = new QSettings(qgetenv("USERPROFILE") + "/Saved Games/MyMineSweeper/savedata.ini", QSettings::IniFormat);

    this->setWindow(this->settings->value("PLAYERSETTING/mineCol", MIN_COL).toInt(),
                    this->settings->value("PLAYERSETTING/mineRow", MIN_ROW).toInt());
    this->scene = new myScene(this->settings->value("PLAYERSETTING/mineCol", MIN_COL).toInt(),
                              this->settings->value("PLAYERSETTING/mineRow", MIN_ROW).toInt(),
                              this->settings->value("PLAYERSETTING/mineNum", MIN_MINE_NUM).toInt());
    this->ui->graphicsView->setScene(scene);
    this->scene->setQmark(this->settings->value("PLAYERSETTING/qmark", true).toBool());
    connect(this->scene, SIGNAL(endGame(bool)), this, SLOT(setRecord(bool)));
    this->group = new QActionGroup(this);
    this->group->addAction(this->ui->action_Beginner);
    this->group->addAction(this->ui->action_Intermediate);
    this->group->addAction(this->ui->action_Elite);
    this->group->addAction(this->ui->action_Custom);
    if(this->settings->value("PLAYERSETTING/mode", "beginner").toString() == "beginner"){
        this->ui->action_Beginner->setChecked(true);
    }else if(this->settings->value("PLAYERSETTING/mode", "intermediate").toString() == "intermediate"){
        this->ui->action_Intermediate->setChecked(true);
    }else if(this->settings->value("PLAYERSETTING/mode", "elite").toString() == "elite"){
        this->ui->action_Elite->setChecked(true);
    }else{
        this->ui->action_Custom->setChecked(true);
    }
    this->ui->action_Mark->setChecked(this->settings->value("PLAYERSETTING/qmark", true).toBool());

    connect(this->ui->action_New_Game, SIGNAL(triggered(bool)), this, SLOT(newGame()));
    connect(this->ui->action_Beginner, SIGNAL(triggered(bool)), this, SLOT(setBeginner()));
    connect(this->ui->action_Intermediate, SIGNAL(triggered(bool)), this, SLOT(setIntermediate()));
    connect(this->ui->action_Elite, SIGNAL(triggered(bool)), this, SLOT(setElite()));
    connect(this->ui->action_Custom, SIGNAL(triggered(bool)), this, SLOT(popupSettings()));
    connect(this->ui->action_Mark, SIGNAL(triggered(bool)), this, SLOT(setQmark(bool)));
    connect(this->ui->action_Time_Records, SIGNAL(triggered(bool)), this, SLOT(popupRecords()));
    connect(this->ui->actionE_xit, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(this->ui->action_About, SIGNAL(triggered(bool)), this, SLOT(popupInfo()));

    settingsDialog = nullptr;
    recordsDialog = nullptr;
    winDialog = nullptr;
    rankingDialog = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete group;
    delete settings;
    //delete scene;
}

void MainWindow::setWindow(int col, int row)
{
    this->setFixedSize(col * GRID + GAP3*3 + GAP6 + GAP5, row * GRID + Field_y + GAP3 + GAP5 + this->ui->menuBar->height());
    this->ui->graphicsView->setFixedSize(GAP3*3 + GAP6 + GAP5 + GRID * col, Field_y + GAP3 + GAP5 + GRID * row + 1);
    this->ui->graphicsView->setSceneRect(0, 0, col * GRID + GAP3*3 + GAP6 + GAP5, row * GRID + Field_y + GAP3 + GAP5);
}

void MainWindow::customGame(int col, int row, int num)
{
    this->scene->setGrid(col, row, num);
    this->setWindow(col, row);
    this->settings->setValue("PLAYERSETTING/mineCol", col);
    this->settings->setValue("PLAYERSETTING/mineRow", row);
    this->settings->setValue("PLAYERSETTING/mineNum", num);
}

void MainWindow::newGame()
{
    this->scene->restartMine();
}

void MainWindow::setBeginner()
{
    this->customGame(9, 9, 10);
    this->settings->setValue("PLAYERSETTING/mode", "beginner");
    this->ui->action_Custom->setChecked(false);
}

void MainWindow::setIntermediate()
{
    this->customGame(16, 16, 40);
    this->settings->setValue("PLAYERSETTING/mode", "intermediate");
    this->ui->action_Custom->setChecked(false);
}

void MainWindow::setElite()
{
    this->customGame(30, 16, 99);
    this->settings->setValue("PLAYERSETTING/mode", "elite");
    this->ui->action_Custom->setChecked(false);
}

void MainWindow::setCustom(int col, int row, int num)
{
    if(col < MIN_COL){
       col = MIN_COL;
    }else if(col > MAX_COL){
       col = MAX_COL;
    }
    if(row < MIN_ROW){
        row = MIN_ROW;
    }else if(row > MAX_ROW){
        row = MAX_ROW;
    }
    if(num < MIN_MINE_NUM){
        num = MIN_MINE_NUM;
    }else if(num > MAX_MINE_NUM(col, row)){
        num = MAX_MINE_NUM(col, row);
    }
    this->customGame(col, row, num);
    this->settings->setValue("PLAYERSETTING/mineCol", col);
    this->settings->setValue("PLAYERSETTING/mineRow", row);
    this->settings->setValue("PLAYERSETTING/mineNum", num);
    this->settings->setValue("PLAYERSETTING/mode", "custom");
    this->ui->action_Custom->setChecked(true);
    this->ui->action_Beginner->setChecked(false);
    this->ui->action_Intermediate->setChecked(false);
    this->ui->action_Elite->setChecked(false);
}

void MainWindow::setQmark(bool flag)
{
    this->scene->setQmark(flag);
    this->settings->setValue("PLAYERSETTING/qmark", flag);
}

void MainWindow::setRecord(bool flag)
{
    if(!flag){
        if(this->settings->value("PLAYERSETTING/mode", "beginner").toString() != "custom"){
            QString key = "PLAYERDATA/" + this->settings->value("PLAYERSETTING/mode", "beginner").toString() + "Record";
            if(this->settings->value(key, 999).toInt() > this->scene->getTime()){
                this->settings->setValue(key, this->scene->getTime());
                this->winDialog = new WinDialog();
                connect(this->winDialog, SIGNAL(accepted()), this, SLOT(popupRecords()));
                this->winDialog->show();
            }
            if(this->settings->value("PLAYERSETTING/mode", "beginner").toString() == "elite"){
                for(int i = 1; i < 10; i++){
                    if(this->settings->value("PLAYERDATA/rank" + key.setNum(i), 999).toInt() > this->scene->getTime()){
                        for(int j = 9; j > i; j--){
                            this->settings->setValue("PLAYERDATA/rank" + key.setNum(j), this->settings->value("PLAYERDATA/rank" + key.setNum(j - 1), 999).toInt());
                        }
                        this->settings->setValue("PLAYERDATA/rank" + key.setNum(i), this->scene->getTime());
                        break;
                    }
                }
            }
        } 
    }
}

void MainWindow::popupSettings()
{
    QString mode = this->settings->value("PLAYERSETTING/mode", "beginner").toString();
    if(mode == "beginner"){
        this->ui->action_Beginner->setChecked(true);
    }else if(mode == "intermediate"){
        this->ui->action_Intermediate->setChecked(true);
    }else if(mode == "elite"){
        this->ui->action_Elite->setChecked(true);
    }
    this->settingsDialog = new Settings(this);
    connect(this->settingsDialog, SIGNAL(settings(int,int,int)), this, SLOT(setCustom(int, int, int)));
    this->settingsDialog->show();
}

void MainWindow::popupRecords()
{
    this->recordsDialog = new Records(this);
    this->recordsDialog->show();
}

void MainWindow::popupInfo()
{
    this->rankingDialog = new Ranking();
    this->rankingDialog->show();
}

