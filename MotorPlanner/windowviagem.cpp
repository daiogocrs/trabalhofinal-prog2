#include "windowviagem.h"
#include "ui_windowviagem.h"
#include "novaviagemdialog.h"

#include <QFile>
#include <QTextStream> // Usaremos QTextStream para ler e escrever texto facilmente
#include <QMessageBox> // Para mostrar mensagens de erro
#include <QDebug>

windowviagem::windowviagem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::windowviagem)
{
    ui->setupUi(this);

    connect(ui->listaViagensWidget, &QListWidget::itemClicked, this, &windowviagem::on_listaViagensWidget_itemClicked);
    connect(ui->criarViagemButton, &QPushButton::clicked, this, &windowviagem::on_criarViagemButton_clicked);

    // Carrega as viagens do nosso novo ficheiro de texto
    carregarViagensDeArquivo();
}

windowviagem::~windowviagem()
{
    // Salva as viagens no nosso novo ficheiro de texto antes de fechar
    salvarViagensParaArquivo();
    delete ui;
}

void windowviagem::on_listaViagensWidget_itemClicked(QListWidgetItem *item)
{
    int index = ui->listaViagensWidget->row(item);

    if (index >= 0 && index < m_viagens.size()) {
        const Viagem& viagemSelecionada = m_viagens.at(index);

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
        ui->listaViagensWidget->addItem(novaViagem.nome);
    }
}

// --- NOVA LÓGICA DE CARREGAMENTO ---
void windowviagem::carregarViagensDeArquivo()
{
    QFile arquivo("viagens.txt");
    if (!arquivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Se o ficheiro não existir, não há problema, ele será criado ao fechar.
        return;
    }

    m_viagens.clear();
    ui->listaViagensWidget->clear();

    QTextStream in(&arquivo);
    while (!in.atEnd()) {
        QString linha = in.readLine();
        QStringList campos = linha.split('|'); // Separa a linha pelo caractere '|'

        if (campos.size() == 4) { // Garante que a linha tem todos os 4 campos
            Viagem v;
            v.nome = campos[0];
            v.destino = campos[1];
            v.data = campos[2];
            // A descrição pode conter novas linhas, então tratamos de forma especial
            v.descricao = campos[3].replace(";;;", "\n"); // Restaura as quebras de linha

            m_viagens.append(v);
            ui->listaViagensWidget->addItem(v.nome);
        }
    }
    arquivo.close();
}

// --- NOVA LÓGICA PARA SALVAR ---
void windowviagem::salvarViagensParaArquivo()
{
    QFile arquivo("viagens.txt");
    if (!arquivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erro", "Não foi possível salvar as viagens no ficheiro viagens.txt");
        return;
    }

    QTextStream out(&arquivo);
    for (const auto& viagem : m_viagens) {
        // Para evitar problemas com quebras de linha na descrição, substituímo-las
        // por um marcador especial (;;;) antes de salvar.
        QString descricaoTratada = viagem.descricao;
        descricaoTratada.replace("\n", ";;;");

        out << viagem.nome << "|"
            << viagem.destino << "|"
            << viagem.data << "|"
            << descricaoTratada << "\n";
    }
    arquivo.close();
}
