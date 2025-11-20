#include "models/nota/Nota.h"

int Nota::getId() {
    return id;
}

int Nota::getFormacaoId() {
    return formacaoId;
}

std::string Nota::getTexto() {
    return texto;
}

Data Nota::getData() {
    return data;
}

void Nota::setFormacaoId(int id) {
    this->id = id;
}

void Nota::setTexto(std::string texto) {
    this->texto = texto;
}

void Nota::setData(Data data) {
    this->data = data;
}