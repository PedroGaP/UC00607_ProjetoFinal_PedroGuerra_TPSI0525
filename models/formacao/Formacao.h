#pragma once
#include <iostream>
#include "utils/utils.h"

inline int formacaoIdIncrement = 1;

struct Formacao {
    private:
        int id;
        std::string nome;
        Data dataConclusao;

    public:
        Formacao() = default;
        Formacao(int id, std::string nome, Data dataConclusao) : id(id), nome(nome), dataConclusao(dataConclusao){}

        int getId();
        std::string getNome();
        Data getDataConclusao();

    void setNome(std::string nome);
    void setDataConclusao(Data dataConclusao);
};