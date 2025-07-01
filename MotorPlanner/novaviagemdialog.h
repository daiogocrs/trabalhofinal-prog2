#ifndef NOVAVIAGEMDIALOG_H
#define NOVAVIAGEMDIALOG_H

#include <QDialog>
#include "viagem.h"

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

    // NOVA FUNÇÃO para preencher o diálogo para edição
    void setViagem(const Viagem& viagem);

private slots:
    void onSelecionarImagemClicked();

private:
    QLineEdit *m_nomeLineEdit;
    QLineEdit *m_destinoLineEdit;
    QDateEdit *m_dataDateEdit;
    QTextEdit *m_descricaoTextEdit;
    QPushButton *m_selecionarImagemButton;
    QLabel *m_caminhoImagemLabel;
    QString m_caminhoImagem;
};

#endif // NOVAVIAGEMDIALOG_H
