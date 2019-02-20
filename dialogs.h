#ifndef DIALOGS_H
#define DIALOGS_H

#include <QDialog>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QSettings>
#include "minemap.h"
#include <QCloseEvent>

namespace Ui {
class Settings;
class Records;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

public slots:
    void getSettings();

signals:
    void settings(int, int, int);

protected:
    void closeEvent(QCloseEvent * event);

private:
    Ui::Settings * ui;
};

class Records : public QDialog
{
    Q_OBJECT

public:
    explicit Records(QWidget *parent = 0);
    ~Records();

public slots:
    void resetRecords();

protected:
    void closeEvent(QCloseEvent * event);

private:
    Ui::Records * ui;
};

class WinDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WinDialog();
    ~WinDialog();
    void setText(QString &);

public slots:
    void setName();

protected:
    void closeEvent(QCloseEvent * event);

private:
    QLabel * label_winInfo;
    QLineEdit * lineEdit_name;
    QPushButton * win_btn_ok;
    QBoxLayout * layout;
};

class Ranking : public QDialog
{
    Q_OBJECT

public:
    explicit Ranking();
    ~Ranking();

public slots:
    void clear();

protected:
    void closeEvent(QCloseEvent * event);

private:
    QString rank[9];
    QLabel * label_rank[9];
    QPushButton * btn_clear;
    QPushButton * btn_ok;
    QBoxLayout * layout;
};

#endif // DIALOGS_H
