#include "windowviagem.h"
#include "ui_windowviagem.h"
#include "novaviagemdialog.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QTableWidgetItem>
#include <QHeaderView> // Essencial para as melhorias visuais do cabeçalho

windowviagem::windowviagem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::windowviagem)
{
    ui->setupUi(this);

    // --- MELHORIAS VISUAIS DA TABELA ---

    // 1. Faz a última coluna esticar para preencher o espaço em branco
    ui->tabelaViagensWidget->horizontalHeader()->setStretchLastSection(true);

    // 2. Ativa as cores de linha alternadas para melhor leitura
    ui->tabelaViagensWidget->setAlternatingRowColors(true);

    // 3. Aplica uma folha de estilos (QSS) para um look mais profissional
    ui->tabelaViagensWidget->setStyleSheet(
        "QTableWidget {"
        "    gridline-color: #e0e0e0;"             /* Cor da grelha mais suave */
        "    font-size: 11pt;"                      /* Tamanho da fonte */
        "}"
        "QHeaderView::section {"
        "    background-color: #f0f0f0;"          /* Cor de fundo do cabeçalho */
        "    padding: 4px;"
        "    border: 1px solid #d0d0d0;"
        "    font-weight: bold;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #a8d8ff;"          /* Cor de fundo da seleção (azul mais vivo) */
        "    color: black;"                       /* Cor do texto quando selecionado */
        "}"
        );

    // --- CONEXÕES DE SINAIS ---
    connect(ui->tabelaViagensWidget, &QTableWidget::itemSelectionChanged, this, &windowviagem::on_tabelaViagensWidget_itemSelectionChanged);
    connect(ui->criarViagemButton, &QPushButton::clicked, this, &windowviagem::on_criarViagemButton_clicked);

    // Carrega as viagens salvas
    carregarViagensDeArquivo();
}

windowviagem::~windowviagem()
{
    salvarViagensParaArquivo();
    delete ui;
}

void windowviagem::on_tabelaViagensWidget_itemSelectionChanged()
{
    int linhaAtual = ui->tabelaViagensWidget->currentRow();

    if (linhaAtual < 0) {
        ui->detalhesViagemTextEdit->clear();
        return;
    }

    if (linhaAtual < m_viagens.size()) {
        const Viagem& viagemSelecionada = m_viagens[linhaAtual];

        QString detalhes = QString("<h1>%1</h1>"
                                   "<b>Destino:</b> %2<br>"

                                   "<b>Data:</b> %3<br><br>"
                                   "<b>Descrição:</b><br>%4")
                               .arg(viagemSelecionada.nome,
                                    viagemSelecionada.destino,
                                    viagemSelecionada.data,
                                    viagemSelecionada.descricao);

        ui->detalhesViagemTextEdit->setHtml(detalhes);
    }
}

void windowviagem::on_criarViagemButton_clicked()
{
    NovaViagemDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Viagem novaViagem = dialog.getViagem();
        m_viagens.append(novaViagem);
        adicionarViagemNaTabela(novaViagem);
    }
}

void windowviagem::adicionarViagemNaTabela(const Viagem& viagem)
{
    int novaLinha = ui->tabelaViagensWidget->rowCount();
    ui->tabelaViagensWidget->insertRow(novaLinha);

    ui->tabelaViagensWidget->setItem(novaLinha, 0, new QTableWidgetItem(viagem.nome));
    ui->tabelaViagensWidget->setItem(novaLinha, 1, new QTableWidgetItem(viagem.destino));
    ui->tabelaViagensWidget->setItem(novaLinha, 2, new QTableWidgetItem(viagem.data));
}

void windowviagem::carregarViagensDeArquivo()
{
    QFile arquivo("viagens.txt");
    if (!arquivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    m_viagens.clear();
    ui->tabelaViagensWidget->setRowCount(0);

    QTextStream in(&arquivo);
    while (!in.atEnd()) {
        QString linha = in.readLine();
        QStringList campos = linha.split('|');

        if (campos.size() == 4) {
            Viagem v;
            v.nome = campos[0];
            v.destino = campos[1];
            v.data = campos[2];
            v.descricao = campos[3].replace(";;;", "\n");

            m_viagens.append(v);
            adicionarViagemNaTabela(v);
        }
    }
    arquivo.close();
}

void windowviagem::salvarViagensParaArquivo()
{
    QFile arquivo("viagens.txt");
    if (!arquivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erro", "Não foi possível salvar as viagens no ficheiro viagens.txt");
        return;
    }

    QTextStream out(&arquivo);
    for (const auto& viagem : m_viagens) {
        QString descricaoTratada = viagem.descricao;
        descricaoTratada.replace("\n", ";;;");

        out << viagem.nome << "|"
            << viagem.destino << "|"
            << viagem.data << "|"
            << descricaoTratada << "\n";
    }
    arquivo.close();
}
