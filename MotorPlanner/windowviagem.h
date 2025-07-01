#ifndef WINDOWVIAGEM_H
#define WINDOWVIAGEM_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "viagem.h" // Inclui a nova definição da Viagem

// Declaração adiantada para evitar o include circular
class NovaViagemDialog;

QT_BEGIN_NAMESPACE
namespace Ui {
class windowviagem;
}
QT_END_NAMESPACE

class windowviagem : public QMainWindow
{
    Q_OBJECT

public:
    windowviagem(QWidget *parent = nullptr);
    ~windowviagem();

private slots:
    void on_listaViagensWidget_itemClicked(QListWidgetItem *item);
    void on_criarViagemButton_clicked();

private:
    void carregarViagensDeArquivo();
    void salvarViagensParaArquivo();

    Ui::windowviagem *ui;
    QList<Viagem> m_viagens;
    NovaViagemDialog *m_novaViagemDialog; // O ponteiro funciona com a declaração adiantada
};
#endif // WINDOWVIAGEM_H
