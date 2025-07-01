#include "novaviagemdialog.h"

// Inclusões para todos os widgets e layouts que vamos criar manualmente
#include <QLineEdit>
#include <QDateEdit>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QFormLayout> // Usaremos um layout de formulário para organizar tudo
#include <QVBoxLayout>
#include <QDate>

NovaViagemDialog::NovaViagemDialog(QWidget *parent) : QDialog(parent)
{
    // --- CRIAÇÃO DOS WIDGETS ---
    m_nomeLineEdit = new QLineEdit(this);
    m_destinoLineEdit = new QLineEdit(this);
    m_dataDateEdit = new QDateEdit(this);
    m_dataDateEdit->setDate(QDate::currentDate()); // Define a data atual como padrão
    m_dataDateEdit->setCalendarPopup(true); // Melhora a usabilidade
    m_dataDateEdit->setDisplayFormat("dd/MM/yyyy");
    m_descricaoTextEdit = new QTextEdit(this);

    // --- CRIAÇÃO DOS BOTÕES OK/CANCELAR ---
    // QDialogButtonBox é uma forma padrão de ter botões OK e Cancelar
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    // --- ORGANIZAÇÃO EM LAYOUTS ---
    // Um layout de formulário é perfeito para pares de "Rótulo: Campo"
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("Nome da Viagem:", m_nomeLineEdit);
    formLayout->addRow("Destino:", m_destinoLineEdit);
    formLayout->addRow("Data:", m_dataDateEdit);
    formLayout->addRow("Descrição:", m_descricaoTextEdit);

    // O layout principal da janela organiza o formulário e os botões verticalmente
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout); // Define o layout principal para o diálogo

    // --- CONEXÃO DOS SINAIS ---
    // Conecta o sinal de 'aceite' do buttonBox (clicar em OK) ao slot 'accept' do diálogo
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    // Conecta o sinal de 'rejeitado' (clicar em Cancelar) ao slot 'reject' do diálogo
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    setWindowTitle("Criar Nova Viagem");
}

Viagem NovaViagemDialog::getViagem() const
{
    Viagem v;
    // Obtém os dados diretamente dos nossos ponteiros de membro
    v.nome = m_nomeLineEdit->text();
    v.destino = m_destinoLineEdit->text();
    v.data = m_dataDateEdit->date().toString("dd/MM/yyyy");
    v.descricao = m_descricaoTextEdit->toPlainText();
    return v;
}
