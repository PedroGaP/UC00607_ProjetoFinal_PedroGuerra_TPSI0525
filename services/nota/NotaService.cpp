#include "NotaService.h"

#include <algorithm>
#include <iomanip>

#include "services/formacao/FormacaoService.h"

void gerirNotasObservacoesColaborador(colabList) {
    header("Gerir Notas / Observacoes");

    Colaborador* colab = escolherColaborador(colabs);

    if (colab == nullptr) {
        return;
    }

    std::cout << "1) Listar Notas" << std::endl;
    std::cout << "2) Adicionar Nota" << std::endl;
    std::cout << "3) Editar Nota" << std::endl;
    std::cout << "4) Remover Nota" << std::endl;
    std::cout << "0) Voltar" << std::endl;
    std::cout << "Escolha uma opcao: " << std::endl;

    int opt = 0;

    if (!(std::cin >> opt)) {
        error(INVALID_OPTION_MSG);
        return;
    }

    switch (opt) {
        case 1:
            listarNotas(colab);
            break;
        case 2:
            adicionarNota(colab);
            break;
        case 3:
            editarNota(colab);
            break;
        case 4:
            removerNota(colab);
            break;
        case 5:
            break;

        default:
            error(INVALID_OPTION_MSG);
            break;
    }
}
void listarNotas(Colaborador* colab) {
    header("Listar Notas (" + colab->getNome() + ")");

    vetor<Nota> *notas = colab->getNotas();

    if (notas->size() == 0) {
        error("Nao existem notas para o colaborador (" + colab->getNome() + ") no sistema.");
        return;
    }

    notas->foreach([](int index, Nota &n) {
        std::cout
            << "ID: " << std::left << std::setw(5) << n.getId()
            << " | Formacao ID: " << std::left << std::setw(5) << n.getFormacaoId()
            << " | Observacao: " << std::left << std::setw(32) << n.getTexto()
            << " | Data: " << std::left << std::setw(10) << n.getData().toString()
            << std::endl;
    });
}

Nota* escolherNota(Colaborador* colab) {
    // Lista as notas
    listarNotas(colab);

    std::cout << std::endl << "Escolha o id da nota do colaborador: ";
    int id = -1;

    // Lê o índice da nnota
    if (!(std::cin >> id)) {
        clearBuffer();
        error(INVALID_OPTION_MSG);
        return nullptr;
    }

    Nota *n = colab->getNotas()->where([&](Nota nota) {
        return nota.getId() == id;
    });

    if (n == nullptr) {
        error("A nota com o id (" + std::to_string(id) + ") nao existe.");
        return nullptr;
    }

    // Retorna a nota escolhida
    return n;
}

void adicionarNota(Colaborador* colab) {
    header("Adicionar Nota ("+ colab->getNome() +")");

    Formacao *f = escolherFormacao(colab);

    if (f == nullptr) {
        return;
    }

    clearBuffer();
    std::string obs;

    std::cout << "Digite a observacao da nota: ";
    std::getline(std::cin, obs);

    int dia, mes, ano;
    std::cout << "Insira a data da nota (dd/mm/aaaa): ";
    // Lê a data
    scanf_s("%d/%d/%d", &dia, &mes, &ano);

    if (obs.empty()) {
        error("Insira um nome valido.");
        return;
    }

    Data d(dia, mes, ano);

    Nota n = Nota(notaIdIncrement, f->getId(), obs, d);

    colab->adicionarNota(n);

    success("Nota adicionada com sucesso!");
}

void editarNota(Colaborador* colab) {
    Nota *n = escolherNota(colab);
    bool modified = false;

    if (n == nullptr) {
        return;
    }

    std::cout << "Deseja mudar o id de formacao da nota? (S/N): ";
    std::string idFormOpt, editarIdForm;

    if (!(std::cin >> idFormOpt)) {
        clearBuffer();
        error(INVALID_OPTION_MSG);
        return;
    }

    std::transform(idFormOpt.begin(), idFormOpt.end(), std::back_inserter(editarIdForm), ::tolower);

    if (editarIdForm.empty() || (editarIdForm != "s" && editarIdForm != "n")) {
        clearBuffer();
        error(INVALID_OPTION_MSG);
        return;
    }

    if (editarIdForm == "s") {

        Formacao *f = escolherFormacao(colab);
        clearBuffer();
        if (f == nullptr) {
            return;
        }
        modified = true;
        n->setFormacaoId(f->getId());
    }

    std::cout << "Deseja mudar a data da nota? (S/N): ";
    std::string dataOpt, editarData;

    if (!(std::cin >> dataOpt)) {
        clearBuffer();
        error(INVALID_OPTION_MSG);
        return;
    }

    std::transform(dataOpt.begin(), dataOpt.end(), std::back_inserter(editarData), ::tolower);

    if (editarData.empty() || (editarData != "s" && editarData != "n")) {
        clearBuffer();
        error(INVALID_OPTION_MSG);
        return;
    }

    if (editarData == "s") {
        clearBuffer();
        std::cout << "Insira a data de conclusao da formacao (dd/mm/aaaa): ";
        std::getline(std::cin, dataOpt);

        if (!validarData(dataOpt)) {
            clearBuffer();
            error("Insira uma data válida.");
            return;
        }
        int dia, mes, ano;

        sscanf_s(dataOpt.c_str(), "%d/%d/%d", &dia, &mes, &ano);

        Data d{dia, mes, ano};

        modified = true;
        n->setData(d);
    }

    std::cout << "Deseja mudar o texto da nota? (S/N): ";
    std::string textoOpt, editarTexto;

    if (!(std::cin >> textoOpt)) {
        clearBuffer();
        error(INVALID_OPTION_MSG);
        return;
    }

    std::transform(textoOpt.begin(), textoOpt.end(), std::back_inserter(editarTexto), ::tolower);

    if (editarTexto.empty() || (editarTexto != "s" && editarTexto != "n")) {
        clearBuffer();
        error(INVALID_OPTION_MSG);
        return;
    }

    if (editarTexto == "s") {
        clearBuffer();
        std::cout << "Insira o texto da nota: ";
        std::getline(std::cin, textoOpt);

        if (textoOpt.empty()) {
            clearBuffer();
            error("Insira um texto valido.");
            return;
        }

        modified = true;
        n->setTexto(textoOpt);
    }

    if (modified) {
        success("A nota com o id (" + std::to_string(n->getId()) + ") foi alterada com sucesso!");
    } else {
        std::cout << "Não houveram alteracoes feitas na nota..." << std::endl;
    }

}

void removerNota(Colaborador* colab) {
    Nota *n = escolherNota(colab);

    if (n == nullptr) {
        return;
    }

    std::cout << "Deseja mesmo remover a nota com o id ("<< n->getId() <<") da formacao com id ("<< n->getFormacaoId() <<") do colaborador ("<< colab->getNome() <<")? (S/N): ";
    std::string rmOpt, rmNota;

    if (!(std::cin >> rmOpt)) {
        clearBuffer();
        error(INVALID_OPTION_MSG);
        return;
    }

    std::transform(rmOpt.begin(), rmOpt.end(), std::back_inserter(rmNota), ::tolower);

    if (rmNota.empty() || (rmNota != "s" && rmNota != "n")) {
        clearBuffer();
        error(INVALID_OPTION_MSG);
        return;
    }

    if (rmNota == "s") {
        vetor<Nota> *notas = colab->getNotas();

        Nota* removed = notas->removeWhere([&](int index, Nota* nota) {
            return nota->getId() == n->getId();
        });

        if (removed == nullptr) {
            clearBuffer();
            error("Ocorreu um erro ao apagar a nota com o id ("+ std::to_string(n->getId()) +").");
            return;
        }

        success("A nota com o id ("+ std::to_string(n->getId()) +") foi removida com sucesso!");
    } else {
        std::cout << "Nao houveram alteracoes feitas na nota..." << std::endl;
    }

}