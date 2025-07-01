#include "windowviagem.h"
#include "ui_windowviagem.h"
#include "novaviagemdialog.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QWidget>

windowviagem::windowviagem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::windowviagem)
{
    ui->setupUi(this);

    // --- CRIAÇÃO DO LAYOUT COM SPLITTER ---
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(ui->tabelaViagensWidget);
    QWidget *painelDireito = new QWidget();
    QVBoxLayout *layoutDireito = new QVBoxLayout(painelDireito);
    layoutDireito->addWidget(ui->detalhesViagemTextEdit, 1);
    layoutDireito->addWidget(ui->imagemLabel, 1);
    painelDireito->setLayout(layoutDireito);
    splitter->addWidget(painelDireito);

    // --- AJUSTE DE TAMANHO DA TABELA ---
    // Aumentei o primeiro valor para dar mais espaço à tabela.
    splitter->setSizes({400, 550});

    QVBoxLayout *layoutPrincipal = new QVBoxLayout();
    layoutPrincipal->addWidget(splitter);
    layoutPrincipal->addWidget(ui->criarViagemButton);
    ui->centralwidget->setLayout(layoutPrincipal);

    // --- NOVA ESTILIZAÇÃO AVANÇADA ---
    ui->tabelaViagensWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tabelaViagensWidget->setAlternatingRowColors(true);
    ui->tabelaViagensWidget->setStyleSheet(R"(
        QTableWidget {
            background-color: #ffffff;
            alternate-background-color: #f7f7f7; /* Cor de fundo alternada suave */
            gridline-color: #e0e0e0;
            font-size: 11pt;
            border: 1px solid #dcdcdc;
        }
        QTableWidget::item {
            padding: 5px;
        }
        QTableWidget::item:hover {
            background-color: #e6f2ff; /* Efeito de hover azul claro */
        }
        QTableWidget::item:selected {
            background-color: #a8d8ff;
            color: #000000;
        }
        QHeaderView::section {
            background-color: #f0f0f0;
            padding: 5px;
            border-top: 0px;
            border-bottom: 1px solid #d0d0d0;
            border-right: 1px solid #d0d0d0;
            font-weight: bold;
        }
        QScrollBar:vertical {
            border: none;
            background: #f0f0f0;
            width: 8px;
            margin: 0px 0px 0px 0px;
        }
        QScrollBar::handle:vertical {
            background: #c0c0c0;
            min-height: 20px;
            border-radius: 4px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
    )");

    // --- Conexões de sinais ---
    connect(ui->tabelaViagensWidget, &QTableWidget::itemSelectionChanged, this, &windowviagem::on_tabelaViagensWidget_itemSelectionChanged);
    connect(ui->criarViagemButton, &QPushButton::clicked, this, &windowviagem::on_criarViagemButton_clicked);

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
        ui->imagemLabel->clear();
        ui->imagemLabel->setText("Selecione uma viagem");
        return;
    }
    if (linhaAtual < m_viagens.size()) {
        const Viagem& viagemSelecionada = m_viagens[linhaAtual];
        QString detalhes = QString("<h1>%1</h1><b>Destino:</b> %2<br><b>Data:</b> %3<br><br><b>Descrição:</b><br>%4")
                               .arg(viagemSelecionada.nome, viagemSelecionada.destino, viagemSelecionada.data, viagemSelecionada.descricao);
        ui->detalhesViagemTextEdit->setHtml(detalhes);
        if (!viagemSelecionada.caminhoImagem.isEmpty() && QFile::exists(viagemSelecionada.caminhoImagem)) {
            QPixmap pixmapOriginal(viagemSelecionada.caminhoImagem);
            ui->imagemLabel->setPixmap(pixmapOriginal.scaled(ui->imagemLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            ui->imagemLabel->clear();
            ui->imagemLabel->setText("Sem imagem.");
        }
    }
}

void windowviagem::on_criarViagemButton_clicked()
{
    NovaViagemDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
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
    if (!arquivo.open(QIODevice::ReadOnly | QIODevice::Text)) { return; }
    m_viagens.clear();
    ui->tabelaViagensWidget->setRowCount(0);
    QTextStream in(&arquivo);
    while (!in.atEnd()) {
        QStringList campos = in.readLine().split('|');
        if (campos.size() == 5) {
            Viagem v;
            v.nome = campos[0];
            v.destino = campos[1];
            v.data = campos[2];
            v.descricao = campos[3].replace(";;;", "\n");
            v.caminhoImagem = campos[4];
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
        out << viagem.nome << "|" << viagem.destino << "|" << viagem.data << "|" << descricaoTratada << "|" << viagem.caminhoImagem << "\n";
    }
    arquivo.close();
}
