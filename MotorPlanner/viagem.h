#ifndef VIAGEM_H
#define VIAGEM_H

#include <QString>

// Estrutura para armazenar os dados de uma viagem
struct Viagem {
    QString nome;
    QString destino;
    QString data;
    QString descricao;
    QString caminhoImagem; // NOVO CAMPO
};

#endif // VIAGEM_H
