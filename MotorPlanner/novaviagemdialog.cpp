#include "novaviagemdialog.h"

#include <QLineEdit>
#include <QDateEdit>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QDate>
#include <QPushButton> // Novas inclusões
#include <QLabel>
#include <QFileDialog> // Para a janela de seleção de ficheiro
#include <QFileInfo>

NovaViagemDialog::NovaViagemDialog(QWidget *parent) : QDialog(parent)
{
    // ... criação dos widgets existentes ...
    m_nomeLineEdit = new QLineEdit(this);
    m_destinoLineEdit = new QLineEdit(this);
    m_dataDateEdit = new QDateEdit(this);
    m_dataDateEdit->setDate(QDate::currentDate());
    m_dataDateEdit->setCalendarPopup(true);
    m_dataDateEdit->setDisplayFormat("dd/MM/yyyy");
    m_descricaoTextEdit = new QTextEdit(this);

    // --- NOVO: Criação dos widgets de imagem ---
    m_selecionarImagemButton = new QPushButton("Selecionar Imagem...", this);
    m_caminhoImagemLabel = new QLabel("Nenhuma imagem selecionada.", this);
    m_caminhoImagemLabel->setWordWrap(true); // Permite que o texto quebre a linha

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("Nome da Viagem:", m_nomeLineEdit);
    formLayout->addRow("Destino:", m_destinoLineEdit);
    formLayout->addRow("Data:", m_dataDateEdit);
    formLayout->addRow("Descrição:", m_descricaoTextEdit);
    // --- NOVO: Adiciona a linha de imagem ao formulário ---
    formLayout->addRow(m_selecionarImagemButton, m_caminhoImagemLabel);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    // Conexão dos botões OK/Cancelar
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    // --- NOVO: Conexão do botão de selecionar imagem ---
    connect(m_selecionarImagemButton, &QPushButton::clicked, this, &NovaViagemDialog::onSelecionarImagemClicked);

    setWindowTitle("Criar Nova Viagem");
}

Viagem NovaViagemDialog::getViagem() const
{
    Viagem v;
    // ... obter dados existentes ...
    v.nome = m_nomeLineEdit->text();
    v.destino = m_destinoLineEdit->text();
    v.data = m_dataDateEdit->date().toString("dd/MM/yyyy");
    v.descricao = m_descricaoTextEdit->toPlainText();
    // --- NOVO: Obter o caminho da imagem ---
    v.caminhoImagem = m_caminhoImagem;
    return v;
}

// --- NOVO: Slot que abre a janela para selecionar o ficheiro ---
void NovaViagemDialog::onSelecionarImagemClicked()
{
    QString caminho = QFileDialog::getOpenFileName(this, "Selecionar Imagem", "", "Ficheiros de Imagem (*.png *.jpg *.jpeg *.bmp)");
    if (!caminho.isEmpty()) {
        m_caminhoImagem = caminho;
        // Mostra apenas o nome do ficheiro para não ocupar muito espaço
        m_caminhoImagemLabel->setText(QFileInfo(caminho).fileName());
    }
}
