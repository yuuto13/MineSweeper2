#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "myscene.h"
#include "dialogs.h"
#include <QActionGroup>
#include <QSettings>
#include <QFile>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setWindow(int col, int row);
    void customGame(int col, int row, int num);

private slots:
    void newGame();
    void setBeginner();
    void setIntermediate();
    void setElite();
    void setCustom(int col, int row, int num);
    void setQmark(bool flag);
    void setRecord(bool flag);
    void popupSettings();
    void popupRecords();
    void popupInfo();

private:
    Ui::MainWindow * ui;
    myScene * scene;
    QActionGroup * group;
    QSettings * settings;
    Settings * settingsDialog;
    Records * recordsDialog;
    WinDialog * winDialog;
    Ranking * rankingDialog;
};

#endif // MAINWINDOW_H
