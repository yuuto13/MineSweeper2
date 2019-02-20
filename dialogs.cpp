#include "dialogs.h"
#include "ui_settings.h"
#include "ui_records.h"
#include <QDebug>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    this->setModal(true);
    //this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(200, 128);
    QSettings qsettings(qgetenv("USERPROFILE") + "/Saved Games/MyMineSweeper/savedata.ini", QSettings::IniFormat);
    this->ui->lineEdit_col->setText(qsettings.value("PLAYERSETTING/mineCol", MIN_COL).toString());
    this->ui->lineEdit_row->setText(qsettings.value("PLAYERSETTING/mineRow", MIN_ROW).toString());
    this->ui->lineEdit_num->setText(qsettings.value("PLAYERSETTING/mineNum", MIN_MINE_NUM).toString());
    connect(this->ui->btn_ok, SIGNAL(clicked(bool)), this, SLOT(getSettings()));
    connect(this->ui->btn_ok, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(this->ui->btn_cancel, SIGNAL(clicked(bool)), this, SLOT(close()));
}

Settings::~Settings()
{
    delete ui;
}

void Settings::getSettings()
{
    int col = this->ui->lineEdit_col->text().toInt();
    int row = this->ui->lineEdit_row->text().toInt();
    int num = this->ui->lineEdit_num->text().toInt();
    emit settings(col, row, num);
}

void Settings::closeEvent(QCloseEvent * event)
{
    delete this;
}

Records::Records(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Records)
{
    ui->setupUi(this);
    this->setModal(true);
    this->setWindowFlags(Qt::Tool);
    this->setFixedSize(256, 112);
    QSettings qsettings(qgetenv("USERPROFILE") + "/Saved Games/MyMineSweeper/savedata.ini", QSettings::IniFormat);
    this->ui->label_beginner->setText(qsettings.value("PLAYERDATA/beginnerRecord", 999).toString() + "s");
    this->ui->label_player_1->setText(qsettings.value("PLAYERDATA/beginnerPlayerName", "Anonymous").toString());
    this->ui->label_intermediate->setText(qsettings.value("PLAYERDATA/intermediateRecord", 999).toString() + "s");
    this->ui->label_player_2->setText(qsettings.value("PLAYERDATA/intermediatePlayerName", "Anonymous").toString());
    this->ui->label_elite->setText(qsettings.value("PLAYERDATA/eliteRecord", 999).toString() + "s");
    this->ui->label_player_3->setText(qsettings.value("PLAYERDATA/elitePlayerName", "Anonymous").toString());
    connect(this->ui->btn_reset, SIGNAL(clicked(bool)), this, SLOT(resetRecords()));
    connect(this->ui->btn_ok, SIGNAL(clicked(bool)), this, SLOT(close()));
}

Records::~Records()
{
    delete ui;
}

void Records::resetRecords()
{
    this->ui->label_beginner->setText("999s");
    this->ui->label_intermediate->setText("999s");
    this->ui->label_elite->setText("999s");
    this->ui->label_player_1->setText("Anonymous");
    this->ui->label_player_2->setText("Anonymous");
    this->ui->label_player_3->setText("Anonymous");
    this->update();
    QSettings qsettings(qgetenv("USERPROFILE") + "/Saved Games/MyMineSweeper/savedata.ini", QSettings::IniFormat);
    qsettings.remove("PLAYERDATA/beginnerRecord");
    qsettings.remove("PLAYERDATA/intermediateRecord");
    qsettings.remove("PLAYERDATA/eliteRecord");
    qsettings.remove("PLAYERDATA/beginnerPlayerName");
    qsettings.remove("PLAYERDATA/intermediatePlayerName");
    qsettings.remove("PLAYERDATA/elitePlayerName");
}

void Records::closeEvent(QCloseEvent * event)
{
    delete this;
}

WinDialog::WinDialog()
{
    QSettings qsettings(qgetenv("USERPROFILE") + "/Saved Games/MyMineSweeper/savedata.ini", QSettings::IniFormat);
    this->label_winInfo = new QLabel("You set a new record\nin " + qsettings.value("PLAYERSETTING/mode", "beginner").toString() + "!\nWhat's your name?", this);
    this->lineEdit_name = new QLineEdit("Anonymous", this);
    this->win_btn_ok = new QPushButton(QStringLiteral("OK"), this);
    this->layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    this->layout->addWidget(this->label_winInfo);
    this->layout->addWidget(this->lineEdit_name);
    this->layout->addWidget(this->win_btn_ok);
    this->setLayout(this->layout);
    this->setModal(true);
    //this->winDialog->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(160, 175);
    connect(this->win_btn_ok, SIGNAL(clicked(bool)), this, SLOT(setName()));
}

WinDialog::~WinDialog()
{
    delete label_winInfo;
    delete lineEdit_name;
    delete win_btn_ok;
    delete layout;
}

void WinDialog::setName()
{
    QSettings qsettings(qgetenv("USERPROFILE") + "/Saved Games/MyMineSweeper/savedata.ini", QSettings::IniFormat);
    QString key = "PLAYERDATA/" + qsettings.value("PLAYERSETTING/mode", "beginner").toString() + "PlayerName";
    qsettings.setValue(key, this->lineEdit_name->text());
    this->close();
    emit this->accepted();
}

void WinDialog::closeEvent(QCloseEvent * event)
{
    delete this;
}

Ranking::Ranking()
{
    QSettings qsettings(qgetenv("USERPROFILE") + "/Saved Games/MyMineSweeper/savedata.ini", QSettings::IniFormat);
    this->btn_clear = new QPushButton(QStringLiteral("Clear"), this);
    this->btn_ok = new QPushButton(QStringLiteral("OK"), this);
    this->layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    for(int i = 0; i < 9; i++){
        rank[i] = qsettings.value(QString("PLAYERDATA/rank%1").arg(i+1), 999).toString();
        this->label_rank[i] = new QLabel(QString("%1. ").arg(i+1) + rank[i], this);
        this->layout->addWidget(this->label_rank[i]);
    }
    this->layout->addWidget(this->btn_clear);
    this->layout->addWidget(this->btn_ok);
    this->setLayout(this->layout);
    this->setFixedSize(100, 350);
    connect(this->btn_clear, SIGNAL(clicked(bool)), this, SLOT(clear()));
    connect(this->btn_ok, SIGNAL(clicked(bool)), this, SLOT(close()));
}

Ranking::~Ranking()
{
    delete btn_clear;
    delete btn_ok;
    delete layout;
    for(int i = 0; i < 9; i++){
        delete label_rank[i];
    }
}

void Ranking::clear()
{
    QSettings qsettings(qgetenv("USERPROFILE") + "/Saved Games/MyMineSweeper/savedata.ini", QSettings::IniFormat);
    for(int i = 0; i < 8; i++){
        rank[i] = rank[i+1];
        qsettings.setValue(QString("PLAYERDATA/rank%1").arg(i+1), rank[i]);
        this->label_rank[i]->setText(QString("%1. ").arg(i+1) + rank[i]);
    }
    qsettings.setValue("PLAYERDATA/rank9", 999);
    this->label_rank[8]->setText("9. 999");
}

void Ranking::closeEvent(QCloseEvent * event)
{
    delete this;
}
