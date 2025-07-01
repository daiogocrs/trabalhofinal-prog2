#include "windowviagem.h"
#include "ui_windowviagem.h"
#include "novaviagemdialog.h" // <<< ADICIONE ESTE INCLUDE AQUI
#include <QDialog>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

windowviagem::windowviagem(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::windowviagem)
{
    ui->setupUi(this);

    // Inicializa a janela de diálogo para que ela possa ser aberta depois
    m_novaViagemDialog = new NovaViagemDialog(this);

    // Conecta os sinais dos widgets aos slots correspondentes
    connect(ui->listaViagensWidget, &QListWidget::itemClicked, this, &windowviagem::on_listaViagensWidget_itemClicked);
    connect(ui->criarViagemButton, &QPushButton::clicked, this, &windowviagem::on_criarViagemButton_clicked);

    // Ao iniciar o programa, carrega as viagens salvas do arquivo
    carregarViagensDeArquivo();
}

windowviagem::~windowviagem()
{
    // Antes de fechar a aplicação, salva todas as viagens no arquivo
    salvarViagensParaArquivo();
    delete ui;
}

// Slot executado quando um item da lista é clicado
void windowviagem::on_listaViagensWidget_itemClicked(QListWidgetItem *item)
{
    int index = ui->listaViagensWidget->row(item);

    // Garante que o índice selecionado é válido
    if (index >= 0 && index < m_viagens.size()) {
        const Viagem& viagemSelecionada = m_viagens[index];

        // Formata o texto com os detalhes da viagem usando HTML básico
        QString detalhes = QString("<h1>%1</h1>"
                                   "<b>Destino:</b> %2<br>"

                                   "<b>Data:</b> %3<br><br>"
                                   "<b>Descrição:</b><br>%4")
                               .arg(viagemSelecionada.nome,
                                    viagemSelecionada.destino,
                                    viagemSelecionada.data,
                                    viagemSelecionada.descricao);

        // Exibe os detalhes formatados no campo de texto à direita
        ui->detalhesViagemTextEdit->setHtml(detalhes);
    }
}

// Slot executado quando o botão "Criar Nova Viagem" é clicado
void windowviagem::on_criarViagemButton_clicked()
{
    // Abre a janela de diálogo e pausa a execução até que ela seja fechada
    if (m_novaViagemDialog->exec() == QDialog::Accepted)
    {
        // Se o usuário clicou em "OK", obtém os dados da nova viagem
        Viagem novaViagem = m_novaViagemDialog->getViagem();

        // Adiciona a viagem à lista de dados em memória
        m_viagens.append(novaViagem);

        // Adiciona o nome da nova viagem à lista visível na interface
        ui->listaViagensWidget->addItem(novaViagem.nome);
    }
}

// Carrega as viagens do arquivo viagens.json
void windowviagem::carregarViagensDeArquivo()
{
    QFile arquivo("viagens.json");
    // Tenta abrir o arquivo para leitura; se não conseguir, sai da função
    if (!arquivo.open(QIODevice::ReadOnly)) {
        qWarning("Arquivo de viagens não encontrado. Um novo será criado ao fechar.");
        return;
    }

    // Lê todo o conteúdo do arquivo
    QByteArray dadosSalvos = arquivo.readAll();
    // Converte o texto lido para um documento JSON
    QJsonDocument doc(QJsonDocument::fromJson(dadosSalvos));
    // Pega o array JSON do documento
    QJsonArray arrayViagens = doc.array();

    // Limpa os dados atuais antes de carregar os novos
    m_viagens.clear();
    ui->listaViagensWidget->clear();

    // Itera sobre cada item do array JSON
    for (const QJsonValue &valor : arrayViagens) {
        QJsonObject obj = valor.toObject();
        Viagem v;
        v.nome = obj["nome"].toString();
        v.destino = obj["destino"].toString();
        v.data = obj["data"].toString();
        v.descricao = obj["descricao"].toString();
        // Adiciona a viagem recuperada à lista de dados e à lista da interface
        m_viagens.append(v);
        ui->listaViagensWidget->addItem(v.nome);
    }

    arquivo.close();
}

// Salva a lista de viagens atual no arquivo viagens.json
void windowviagem::salvarViagensParaArquivo()
{
    QJsonArray arrayViagens;
    // Itera sobre cada viagem na lista de dados em memória
    for (const auto& viagem : m_viagens) {
        QJsonObject objViagem;
        objViagem["nome"] = viagem.nome;
        objViagem["destino"] = viagem.destino;
        objViagem["data"] = viagem.data;
        objViagem["descricao"] = viagem.descricao;
        // Adiciona o objeto JSON da viagem ao array JSON
        arrayViagens.append(objViagem);
    }

    QJsonDocument doc(arrayViagens);

    QFile arquivo("viagens.json");
    // Tenta abrir o arquivo para escrita (sobrescrevendo o conteúdo)
    if (!arquivo.open(QIODevice::WriteOnly)) {
        qWarning("Não foi possível abrir o arquivo de viagens para escrita.");
        return;
    }

    // Escreve o documento JSON (convertido para texto) no arquivo
    arquivo.write(doc.toJson());
    arquivo.close();
}
