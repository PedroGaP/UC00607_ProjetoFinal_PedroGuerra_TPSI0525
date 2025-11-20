#include "FormacaoService.h"
#include "utils/utils.h"
#include "../../models/colaborador/Colaborador.h"
#include <algorithm>
#include <iomanip>

// Formacoes
void gerirFormacoesCursosColaborador(colabList) {
	header("Gerir Formacoes / Cursos");

	Colaborador* colab = escolherColaborador(colabs);

	if (colab == nullptr) {
		return;
	}

	std::cout << "1) Listar formacoes" << std::endl;
	std::cout << "2) Adicionar formacao" << std::endl;
	std::cout << "3) Editar Formacao" << std::endl;
	std::cout << "4) Remover Formacao" << std::endl;
	std::cout << "0) Voltar" << std::endl;
	std::cout << "Escolha uma opcao: " << std::endl;

	int opt = 0;

	if (!(std::cin >> opt)) {
		error(INVALID_OPTION_MSG);
		return;
	}

	switch (opt) {
		case 1:
			listarFormacoes(colab);
			break;
		case 2:
			adicionarFormacao(colab);
			break;
		case 3:
			editarFormacao(colab);
			break;
		case 4:
			removerFormacao(colab);
			break;
		case 5:
			break;

		default:
			error(INVALID_OPTION_MSG);
			break;
	}

}

void listarFormacoes(Colaborador* colab) {
	header("Listar Formacoes (" + colab->getNome() + ")");

	vetor<Formacao> *formacoes = colab->getFormacoes();

	if (formacoes->size() == 0) {
		error("Nao existem formacoes para o colaborador (" + colab->getNome() + ") no sistema.");
		return;
	}

	formacoes->foreach([](int index, Formacao &f) {
		std::cout
			<< "ID: " << std::left << std::setw(5) << f.getId()
			<< " | Nome: " << std::left << std::setw(32) << f.getNome()
			<< " | Data Conclusao: " << std::left << std::setw(10) << f.getDataConclusao().toString()
			<< std::endl;
	});
}

Formacao* escolherFormacao(Colaborador* colab) {
	// Lista as formacoes
	listarFormacoes(colab);

	std::cout << std::endl << "Escolha o id da formacao do colaborador: ";
	int id = -1;

	// Lê o índice da formacao
	if (!(std::cin >> id)) {
		clearBuffer();
		error(INVALID_OPTION_MSG);
		return nullptr;
	}

	Formacao *f = colab->getFormacoes()->where([&](Formacao formacao) {
		return formacao.getId() == id;
	});

	if (f == nullptr) {
		error("A formacao com o id (" + std::to_string(id) + ") nao existe.");
		return nullptr;
	}

	// Retorna a formacao escolhida
	return f;
}

void adicionarFormacao(Colaborador* colab) {
	header("Adicionar Formacao ("+ colab->getNome() +")");

	clearBuffer();
	std::string nome;

	std::cout << "Digite o nome da formacao: ";
	std::getline(std::cin, nome);

	int dia, mes, ano;
	std::cout << "Insira a data de conclusao da formacao (dd/mm/aaaa): ";
	// Lê a data
	scanf_s("%d/%d/%d", &dia, &mes, &ano);

	if (nome.empty()) {
		error("Insira um nome valido.");
		return;
	}

	Data d(dia, mes, ano);

	Formacao f = Formacao(formacaoIdIncrement, nome, d);

	colab->adicionarFormacao(f);

	success("Formacao adicionada com sucesso!");
}

void editarFormacao(Colaborador* colab) {
	Formacao *f = escolherFormacao(colab);
	bool modified = false;

	if (f == nullptr) {
		return;
	}

	std::cout << "Deseja mudar o nome da formacao? (S/N): ";
	std::string nomeOpt, editarNome;

	if (!(std::cin >> nomeOpt)) {
		clearBuffer();
		error(INVALID_OPTION_MSG);
		return;
	}

	std::transform(nomeOpt.begin(), nomeOpt.end(), std::back_inserter(editarNome), ::tolower);

	if (editarNome.empty() || (editarNome != "s" && editarNome != "n")) {
		clearBuffer();
		error(INVALID_OPTION_MSG);
		return;
	}

	if (editarNome == "s") {
		clearBuffer();
		std::cout << "Insira o nome da formacao: ";
		std::getline(std::cin, nomeOpt);
		if (nomeOpt.empty()) {
			clearBuffer();
			error("Insira um nome valido.");
			return;
		}
		modified = true;
		f->setNome(nomeOpt);
	}

	std::cout << "Deseja mudar a data de conclusao da formacao? (S/N): ";
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
		f->setDataConclusao(d);
	}

	if (modified) {
		success("A formacao com o id (" + std::to_string(f->getId()) + ") foi alterada com sucesso!");
	} else {
		std::cout << "Não houveram alteracoes feitas na formacao..." << std::endl;
	}

}

void removerFormacao(Colaborador* colab) {
	Formacao *f = escolherFormacao(colab);

	if (f == nullptr) {
		return;
	}

	std::cout << "Deseja mesmo remover a formacao com o id ("<< f->getId() <<") e nome ("<< f->getNome() <<") do colaborador ("<< colab->getNome() <<")? (S/N): ";
	std::string rmOpt, rmFormacao;

	if (!(std::cin >> rmOpt)) {
		clearBuffer();
		error(INVALID_OPTION_MSG);
		return;
	}

	std::transform(rmOpt.begin(), rmOpt.end(), std::back_inserter(rmFormacao), ::tolower);

	if (rmFormacao.empty() || (rmFormacao != "s" && rmFormacao != "n")) {
		clearBuffer();
		error(INVALID_OPTION_MSG);
		return;
	}

	if (rmFormacao == "s") {
		vetor<Formacao> *formacoes = colab->getFormacoes();

		Formacao* removed = formacoes->removeWhere([&](int index, Formacao* form) {
			return form->getId() == f->getId();
		});

		if (removed == nullptr) {
			clearBuffer();
			error("Ocorreu um erro ao apagar a formacao com o id ("+ std::to_string(f->getId()) +").");
			return;
		}

		success("A formacao com o id ("+ std::to_string(f->getId()) +") foi removida com sucesso!");
	} else {
		std::cout << "Nao houveram alteracoes feitas na formacao..." << std::endl;
	}

}