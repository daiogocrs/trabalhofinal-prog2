#include "windowviagem.h"
#include "ui_windowviagem.h"

windowviagem::windowviagem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::windowviagem)
{
    ui->setupUi(this);
}

windowviagem::~windowviagem()
{
    delete ui;
}
