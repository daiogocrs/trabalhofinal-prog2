#ifndef NOVAVIAGEMDIALOG_H
#define NOVAVIAGEMDIALOG_H

#include <QDialog>
#include "viagem.h"

// Pré-declaração dos widgets
class QLineEdit;
class QDateEdit;
class QTextEdit;
class QPushButton;
class QLabel;

class NovaViagemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NovaViagemDialog(QWidget *parent = nullptr);
    Viagem getViagem() const;

private slots:
    // Slot para o nosso novo botão
    void onSelecionarImagemClicked();

private:
    // Ponteiros para os widgets
    QLineEdit *m_nomeLineEdit;
    QLineEdit *m_destinoLineEdit;
    QDateEdit *m_dataDateEdit;
    QTextEdit *m_descricaoTextEdit;

    // Novos widgets para a imagem
    QPushButton *m_selecionarImagemButton;
    QLabel *m_caminhoImagemLabel;
    QString m_caminhoImagem; // Variável para guardar o caminho escolhido
};

#endif // NOVAVIAGEMDIALOG_H
