#ifndef WINDOWVIAGEM_H
#define WINDOWVIAGEM_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class windowviagem;
}
QT_END_NAMESPACE

class windowviagem : public QMainWindow
{
    Q_OBJECT

public:
    windowviagem(QWidget *parent = nullptr);
    ~windowviagem();

private:
    Ui::windowviagem *ui;
};
#endif // WINDOWVIAGEM_H
