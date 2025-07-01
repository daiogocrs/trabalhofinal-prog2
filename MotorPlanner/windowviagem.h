#ifndef WINDOWVIAGEM_H
#define WINDOWVIAGEM_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QList>      // Incluir para usar QList
#include "viagem.h"   // Inclui a definição da estrutura Viagem

// Declaração adiantada da classe do diálogo para evitar inclusão circular
class NovaViagemDialog;

QT_BEGIN_NAMESPACE
namespace Ui { class windowviagem; }
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
    QList<Viagem> m_viagens;          // Lista para guardar os dados das viagens
    NovaViagemDialog *m_novaViagemDialog; // Ponteiro para a janela de diálogo
};
#endif // WINDOWVIAGEM_H
