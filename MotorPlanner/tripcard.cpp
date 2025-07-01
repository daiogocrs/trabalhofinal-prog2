#include "tripcard.h"
#include "ui_tripcard.h"
#include <QPixmap>

TripCard::TripCard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TripCard)
{
    ui->setupUi(this);
}

TripCard::~TripCard()
{
    delete ui;
}

void TripCard::setData(const QString &nomeViagem, const QString &dataLocal, int lotacaoPercentual, const QString &status, const QString &caminhoImagem)
{
    // Preenche os labels com os dados
    ui->nomeViagemLabel->setText(nomeViagem);
    ui->dataLocalLabel->setText(dataLocal);
    ui->statusLabel->setText(status);

    // Configura a barra de progresso
    ui->lotacaoProgressBar->setValue(lotacaoPercentual);

    // Carrega a imagem
    QPixmap pixmap(caminhoImagem);
    if (!pixmap.isNull()) {
        // Redimensiona a imagem para caber no label, mantendo a proporção
        ui->imagemLabel->setPixmap(pixmap.scaled(ui->imagemLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        // Caso a imagem não carregue, mostra uma cor ou texto padrão
        ui->imagemLabel->setText("Imagem não encontrada");
        ui->imagemLabel->setStyleSheet("background-color: #333; color: white;");
    }
}
