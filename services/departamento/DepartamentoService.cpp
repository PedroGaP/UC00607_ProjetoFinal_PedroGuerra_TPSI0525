#include "DepartamentoService.h"

#include <iomanip>
#include <map>

#include "models/colaborador/Colaborador.h"
#include "utils/utils.h"

bool temColaboradorComDiaFerias(colabList, Data d, Colaborador *colab) {
    vetor<Colaborador> colabsDepartamento = colabs->whereRange([&](Colaborador c) {
        return c.getDepartamento() == colab->getDepartamento() && c.getId() != colab->getId();
    });

    if (colabsDepartamento.size() <= 0) return false;

    for (auto colab2: colabsDepartamento) {
        vetor<ItemCalendario> ferias = colab2.getItensCalendario()->whereRange([&](const ItemCalendario& ic) {
            Data dc = parseDate(ic.getData());
            return dc.compare(d);
        });

        if (ferias.size() <= 0) return false;
    }

    return true;
}

void estatisticasDepartamento(colabList) {
    std::map<std::string, int[2]> statsDepartamento;

    colabs->foreach([&](int i, Colaborador c) {
        int ferias = c.getItensCalendario()->whereRange([&](ItemCalendario ic) {
            return ic.getTipo() == FERIAS;
        }).size();

        int faltas = c.getItensCalendario()->whereRange([&](ItemCalendario ic) {
            return ic.getTipo() == FALTA;
        }).size();

        statsDepartamento[c.getDepartamento()][0] += ferias;
        statsDepartamento[c.getDepartamento()][1] += faltas;

    });

    // Imprimir tabela
    std::cout << std::left << std::setw(15) << "Departamento"
              << std::setw(8) << "Ferias"
              << std::setw(8) << "Faltas"
              << std::endl;
    std::cout << std::string(46, '-') << std::endl;

    for (auto &[dep, dados] : statsDepartamento) {
        int feriasTotal = dados[0];
        int faltasTotal = dados[1];

        std::cout << std::left << std::setw(15) << dep
                  << std::setw(8) << feriasTotal
                  << std::setw(8) << faltasTotal
                  << std::endl;
    }

    std::string departamento = "";
    int faltasDepartamento = -1;

    for (auto &[dep, dados] : statsDepartamento) {
        if (faltasDepartamento == -1) {
            departamento = dep;
            faltasDepartamento = dados[1];
        }

        if (dados[1] > faltasDepartamento) {
            departamento = dep;
            faltasDepartamento = dados[1];
        }
    }

    if (faltasDepartamento < 1)  return;

    std::cout << "O departamento que possui maior numero de faltas e o ("<< departamento <<") com ("<< faltasDepartamento <<") faltas." << std::endl;

}



