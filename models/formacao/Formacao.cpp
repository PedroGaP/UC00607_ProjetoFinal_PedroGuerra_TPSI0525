//
// Created by peter on 17/11/2025.
//

#include "Formacao.h"

int Formacao::getId() {
    return id;
}

std::string Formacao::getNome() {
    return nome;
}

Data Formacao::getDataConclusao() {
    return dataConclusao;
}

void Formacao::setNome(std::string nome) {
    this->nome = nome;
}

void Formacao::setDataConclusao(Data dataConclusao) {
    this->dataConclusao = dataConclusao;
}
