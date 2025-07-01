#ifndef TRIPCARD_H
#define TRIPCARD_H

#include <QWidget>
#include <QString>
#include <QDate>

// Se você já criou a classe Viagem, inclua o header dela aqui.
// #include "viagem.h"

namespace Ui {
class TripCard;
}

class TripCard : public QWidget
{
    Q_OBJECT

public:
    explicit TripCard(QWidget *parent = nullptr);
    ~TripCard();

    // Função para configurar os dados do card
    void setData(const QString &nomeViagem, const QString &dataLocal, int lotacaoPercentual, const QString &status, const QString &caminhoImagem);

private:
    Ui::TripCard *ui;
};

#endif // TRIPCARD_H
