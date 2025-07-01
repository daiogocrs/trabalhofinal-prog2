#include "novaviagemdialog.h"
#include "ui_novaviagemdialog.h" // <<< ESTA É A LINHA ESSENCIAL QUE CORRIGE O ERRO

NovaViagemDialog::NovaViagemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NovaViagemDialog)
{
    ui->setupUi(this);
}

NovaViagemDialog::~NovaViagemDialog()
{
    delete ui;
}

// Implementação da função que coleta os dados dos widgets e retorna uma struct Viagem
Viagem NovaViagemDialog::getViagem()
{
    Viagem v;
    v.nome = ui->nomeLineEdit->text();
    v.destino = ui->destinoLineEdit->text();
    v.data = ui->dataDateEdit->date().toString("dd/MM/yyyy");
    v.descricao = ui->descricaoTextEdit->toPlainText();
    return v;
}
