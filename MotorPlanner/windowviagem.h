#ifndef WINDOWVIAGEM_H
#define WINDOWVIAGEM_H

#include <QMainWindow>
#include <QList>
#include "viagem.h"

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
    // Slot atualizado para a seleção de itens na tabela
    void on_tabelaViagensWidget_itemSelectionChanged();
    void on_criarViagemButton_clicked();

private:
    // Função de ajuda para adicionar uma viagem à nova tabela
    void adicionarViagemNaTabela(const Viagem& viagem);

    // Funções de salvar/carregar
    void carregarViagensDeArquivo();
    void salvarViagensParaArquivo();

    Ui::windowviagem *ui;
    QList<Viagem> m_viagens;
};
#endif // WINDOWVIAGEM_H
