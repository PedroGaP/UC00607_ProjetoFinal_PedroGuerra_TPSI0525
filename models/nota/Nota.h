#pragma once
#include <string>
#include "utils/utils.h"

inline int notaIdIncrement = 1;

struct Nota {
    private:
        int id;
        int formacaoId;
        std::string texto;
        Data data;
    public:
        Nota() = default;
        Nota(int id, int formacaoId, std::string texto, Data data) : id(id), formacaoId(formacaoId), texto(texto), data(data) {};

        int getId();
        int getFormacaoId();
        std::string getTexto();
        Data getData();

        void setFormacaoId(int id);
        void setTexto(std::string texto);
        void setData(Data data);
};