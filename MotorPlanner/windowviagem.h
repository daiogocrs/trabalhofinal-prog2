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
    void on_tabelaViagensWidget_itemSelectionChanged();
    void on_criarViagemButton_clicked();
    // NOVOS SLOTS para os botões
    void on_editarViagemButton_clicked();
    void on_excluirViagemButton_clicked();

    void on_actionMinhas_Viagens_triggered();

    void on_actionVers_o_triggered();

    void on_actionDesenvolvedor_triggered();

    void on_actionCriar_Viagem_triggered();

private:
    void adicionarViagemNaTabela(const Viagem& viagem);
    // NOVA FUNÇÃO para atualizar uma linha da tabela
    void atualizarViagemNaTabela(int linha, const Viagem& viagem);
    void carregarViagensDeArquivo();
    void salvarViagensParaArquivo();

    Ui::windowviagem *ui;
    QList<Viagem> m_viagens;
};
#endif // WINDOWVIAGEM_H
