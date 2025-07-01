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

    // --- CRIAÇÃO DO LAYOUT ---
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(ui->tabelaViagensWidget);
    QWidget *painelDireito = new QWidget();
    QVBoxLayout *layoutDireito = new QVBoxLayout(painelDireito);
    layoutDireito->addWidget(ui->detalhesViagemTextEdit, 1);
    layoutDireito->addWidget(ui->imagemLabel, 1);
    painelDireito->setLayout(layoutDireito);
    splitter->addWidget(painelDireito);
    splitter->setSizes({400, 550});

    QHBoxLayout* botoesLayout = new QHBoxLayout();
    botoesLayout->addWidget(ui->criarViagemButton);
    botoesLayout->addWidget(ui->editarViagemButton);
    botoesLayout->addWidget(ui->excluirViagemButton);
    botoesLayout->addStretch();

    QVBoxLayout *layoutPrincipal = new QVBoxLayout();
    layoutPrincipal->addWidget(splitter);
    layoutPrincipal->addLayout(botoesLayout);
    ui->centralwidget->setLayout(layoutPrincipal);

    // --- ESTILIZAÇÃO ---
    ui->tabelaViagensWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tabelaViagensWidget->setAlternatingRowColors(true);
    ui->tabelaViagensWidget->setStyleSheet(R"(
        QTableWidget { background-color: #ffffff; alternate-background-color: #f7f7f7; gridline-color: #e0e0e0; font-size: 11pt; border: 1px solid #dcdcdc; }
        QTableWidget::item { padding: 5px; }
        QTableWidget::item:hover { background-color: #e6f2ff; }
        QTableWidget::item:selected { background-color: #a8d8ff; color: #000000; }
        QHeaderView::section { background-color: #f0f0f0; padding: 5px; border-top: 0px; border-bottom: 1px solid #d0d0d0; border-right: 1px solid #d0d0d0; font-weight: bold; }
        QScrollBar:vertical { border: none; background: #f0f0f0; width: 8px; margin: 0px 0px 0px 0px; }
        QScrollBar::handle:vertical { background: #c0c0c0; min-height: 20px; border-radius: 4px; }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }
    )");

    // --- CONEXÕES DE SINAIS ---
    // Apenas a conexão da tabela é necessária aqui, pois ela não segue a convenção de nome.
    // As conexões dos botões foram REMOVIDAS para evitar a duplicação.
    connect(ui->tabelaViagensWidget, &QTableWidget::itemSelectionChanged, this, &windowviagem::on_tabelaViagensWidget_itemSelectionChanged);

    carregarViagensDeArquivo();
    // Força a chamada inicial para garantir que os botões comecem desativados
    on_tabelaViagensWidget_itemSelectionChanged();
}

windowviagem::~windowviagem() {
    salvarViagensParaArquivo();
    delete ui;
}

void windowviagem::on_tabelaViagensWidget_itemSelectionChanged()
{
    bool temSelecao = ui->tabelaViagensWidget->currentRow() >= 0;
    ui->editarViagemButton->setEnabled(temSelecao);
    ui->excluirViagemButton->setEnabled(temSelecao);

    if (!temSelecao) {
        ui->detalhesViagemTextEdit->clear();
        ui->imagemLabel->clear();
        ui->imagemLabel->setText("Selecione uma viagem");
        return;
    }

    int linhaAtual = ui->tabelaViagensWidget->currentRow();
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

void windowviagem::on_editarViagemButton_clicked()
{
    int linha = ui->tabelaViagensWidget->currentRow();
    if (linha < 0) return;

    NovaViagemDialog dialog(this);
    dialog.setViagem(m_viagens.at(linha));

    if (dialog.exec() == QDialog::Accepted) {
        Viagem viagemAtualizada = dialog.getViagem();
        m_viagens[linha] = viagemAtualizada;
        atualizarViagemNaTabela(linha, viagemAtualizada);
    }
}

void windowviagem::on_excluirViagemButton_clicked()
{
    int linha = ui->tabelaViagensWidget->currentRow();
    if (linha < 0) return;

    QMessageBox::StandardButton resposta = QMessageBox::question(this, "Excluir Viagem",
                                                                 "Tem a certeza que deseja excluir a viagem '" + m_viagens.at(linha).nome + "'?",
                                                                 QMessageBox::Yes | QMessageBox::No);

    if (resposta == QMessageBox::Yes) {
        m_viagens.removeAt(linha);
        ui->tabelaViagensWidget->removeRow(linha);
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

void windowviagem::atualizarViagemNaTabela(int linha, const Viagem &viagem)
{
    ui->tabelaViagensWidget->item(linha, 0)->setText(viagem.nome);
    ui->tabelaViagensWidget->item(linha, 1)->setText(viagem.destino);
    ui->tabelaViagensWidget->item(linha, 2)->setText(viagem.data);
    on_tabelaViagensWidget_itemSelectionChanged();
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
            v.nome = campos[0]; v.destino = campos[1]; v.data = campos[2];
            v.descricao = campos[3].replace(";;;", "\n"); v.caminhoImagem = campos[4];
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
