#ifndef NOVAVIAGEMDIALOG_H
#define NOVAVIAGEMDIALOG_H

#include <QDialog>
#include "viagem.h" // Inclui a definição de Viagem a partir do novo arquivo

namespace Ui {
class NovaViagemDialog;
}

class NovaViagemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NovaViagemDialog(QWidget *parent = nullptr);
    ~NovaViagemDialog();

    // Esta função agora funcionará, pois Viagem está definido
    Viagem getViagem();

private:
    Ui::NovaViagemDialog *ui;
};

#endif // NOVAVIAGEMDIALOG_H
