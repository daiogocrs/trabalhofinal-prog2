#ifndef NOVAVIAGEMDIALOG_H
#define NOVAVIAGEMDIALOG_H

#include <QDialog>
#include "viagem.h" // Inclui a definição da estrutura Viagem

// Pré-declaração dos widgets que vamos usar, em vez da classe Ui
class QLineEdit;
class QDateEdit;
class QTextEdit;
class QDialogButtonBox;

class NovaViagemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NovaViagemDialog(QWidget *parent = nullptr);

    // A função para obter os dados continua a mesma
    Viagem getViagem() const;

private:
    // Em vez de um ponteiro 'ui', teremos ponteiros para cada widget individual
    QLineEdit *m_nomeLineEdit;
    QLineEdit *m_destinoLineEdit;
    QDateEdit *m_dataDateEdit;
    QTextEdit *m_descricaoTextEdit;
};

#endif // NOVAVIAGEMDIALOG_H
