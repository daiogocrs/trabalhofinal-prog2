#include "novaviagemdialog.h"

#include <QLineEdit>
#include <QDateEdit>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QDate>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QFileInfo>

NovaViagemDialog::NovaViagemDialog(QWidget *parent) : QDialog(parent)
{
    // A construção da janela continua igual...
    m_nomeLineEdit = new QLineEdit(this);
    m_destinoLineEdit = new QLineEdit(this);
    m_dataDateEdit = new QDateEdit(this);
    m_dataDateEdit->setDate(QDate::currentDate());
    m_dataDateEdit->setCalendarPopup(true);
    m_dataDateEdit->setDisplayFormat("dd/MM/yyyy");
    m_descricaoTextEdit = new QTextEdit(this);
    m_selecionarImagemButton = new QPushButton("Selecionar Imagem...", this);
    m_caminhoImagemLabel = new QLabel("Nenhuma imagem selecionada.", this);
    m_caminhoImagemLabel->setWordWrap(true);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("Nome da Viagem:", m_nomeLineEdit);
    formLayout->addRow("Destino:", m_destinoLineEdit);
    formLayout->addRow("Data:", m_dataDateEdit);
    formLayout->addRow("Descrição:", m_descricaoTextEdit);
    formLayout->addRow(m_selecionarImagemButton, m_caminhoImagemLabel);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(m_selecionarImagemButton, &QPushButton::clicked, this, &NovaViagemDialog::onSelecionarImagemClicked);
    setWindowTitle("Dados da Viagem");
}

Viagem NovaViagemDialog::getViagem() const
{
    Viagem v;
    v.nome = m_nomeLineEdit->text();
    v.destino = m_destinoLineEdit->text();
    v.data = m_dataDateEdit->date().toString("dd/MM/yyyy");
    v.descricao = m_descricaoTextEdit->toPlainText();
    v.caminhoImagem = m_caminhoImagem;
    return v;
}

// NOVA IMPLEMENTAÇÃO para popular os campos
void NovaViagemDialog::setViagem(const Viagem &viagem)
{
    m_nomeLineEdit->setText(viagem.nome);
    m_destinoLineEdit->setText(viagem.destino);
    m_dataDateEdit->setDate(QDate::fromString(viagem.data, "dd/MM/yyyy"));
    m_descricaoTextEdit->setPlainText(viagem.descricao);
    m_caminhoImagem = viagem.caminhoImagem;

    if (!m_caminhoImagem.isEmpty()) {
        m_caminhoImagemLabel->setText(QFileInfo(m_caminhoImagem).fileName());
    } else {
        m_caminhoImagemLabel->setText("Nenhuma imagem selecionada.");
    }
}

void NovaViagemDialog::onSelecionarImagemClicked()
{
    QString caminho = QFileDialog::getOpenFileName(this, "Selecionar Imagem", "", "Ficheiros de Imagem (*.png *.jpg *.jpeg *.bmp)");
    if (!caminho.isEmpty()) {
        m_caminhoImagem = caminho;
        m_caminhoImagemLabel->setText(QFileInfo(caminho).fileName());
    }
}
