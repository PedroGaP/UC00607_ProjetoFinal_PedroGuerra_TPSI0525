#include "ColaboradorService.h"
#include "models/colaborador/Colaborador.h"
#include "utils/utils.h"
#include <iostream>
#include <conio.h>
#include <iomanip>
#include <filesystem>
#include <chrono>
#include <fstream>
#include <map>
#include "services/departamento/DepartamentoService.h"

// Escolhe um colaborador da lista
// Retorna um ponteiro para o colaborador escolhido ou nullptr se a escolha for invalida
Colaborador* escolherColaborador(colabList) {
	// Lista os colaboradores
	listarColaboradores(colabs);

	std::cout << std::endl << "Escolha o id do colaborador: ";
	int id = -1;

	// Lê o índice do colaborador
	if (!(std::cin >> id)) {
		clearBuffer();
		error(INVALID_OPTION_MSG);
		return nullptr;
	}

	Colaborador *c = colabs->where([&](Colaborador colab) {
		return colab.getId() == id;
	});

	if (c == nullptr) {
		error("O colaborador com o id ("+ std::to_string(id) +") nao existe.");
		return nullptr;
	}

	// Retorna o colaborador escolhido2
	return c;
}

bool adicionarColaborador(colabList) {
	clearBuffer();
	std::cout << std::endl << "Insira o nome do colaborador: ";
	std::string nome;
	std::getline(std::cin, nome);

	std::cout << std::endl << "Insira o departamento do colaborador: ";
	std::string departamento;
	std::getline(std::cin, departamento);

	// Verifica se o nome é válido
	if(nome.empty() || departamento.empty()) {
		error("Nome ou Departamento inválido.");
		return false;
	}

	// Verifica se já existe um colaborador com o mesmo nome
	Colaborador c(++colaboradorIdIncrement, nome, departamento);
	Colaborador *colab = colabs->where([&nome](Colaborador obj) {
		return obj.getNome() == nome;
	});

	// Se já existir, retorna falso
	if(colab != nullptr) {
		error("Ja existe um colaborador com o nome (" + nome + ").");
		colaboradorIdIncrement--;
		return false;
	}

	// Adiciona o colaborador à lista
	colabs->push_back(c);
	c.mostrarDetalhesColaborador();

	return true;
}

void listarColaboradores(colabList) {

	header("Lista de Colaboradores ["+ std::to_string(colabs->size()) +"]");

	// Verifica se há colaboradores na lista
	if (colabs->size() == 0) {
		error(NO_COLABORATORS_MSG);
		return;
	}

	// Lista os colaboradores
	colabs->foreach([](int index, Colaborador& c) {
		c.mostrarDetalhesColaborador();
	});
}

bool desmarcar(Colaborador* colab) {
	std::string dataInicio;
	clearBuffer();
	std::cout << "Insira a data (DD/MM/AAAA) para desmarcar: ";
	std::getline(std::cin, dataInicio);

	// Valida a data
	if (!validarData(dataInicio)) {
		return false;
	}

	// Obtém os itens do calendário do colaborador
	vetor<ItemCalendario>* itens = colab->getItensCalendario();

	// Verifica se há itens no calendário
	if (itens->size() < 1) {
		error("O colaborador ("+ colab->getNome() +") nao possui itens no seu calendario.");
		return false;
	}

	int indexToRemove = -1;
	// Procura o item com a data especificada
	for (int i = 0; i < itens->size(); i++) {
		if (itens->at(i).getData() == dataInicio) {
			indexToRemove = i;
			break;
		}
	}

	// Se não encontrou o item, retorna falso
	if (indexToRemove == -1) {
		error("Nenhum item encontrado na data especificada.");
		return false;
	}

	// Cria um novo vetor sem o item a ser removido
	vetor<ItemCalendario> newItens;

	// Adiciona todos os itens, exceto o que será removido
	for (int i = 0; i < itens->size(); i++) {
		if (i != indexToRemove) {
			newItens.push_back(itens->at(i));
		}
	}

	// Atualiza o vetor de itens do colaborador
	*itens = newItens;
	success("Item removido com sucesso.");
	return true;
}

bool marcar(Colaborador *colab, colabList) {
	int opt = -1;
	header("Marcar Ferias/Faltas (" + colab->getNome() + ")");
	std::cout << "[1] Marcar Ferias" << std::endl;
	std::cout << "[2] Marcar Falta" << std::endl;
	std::cout << "[0] Voltar" << std::endl;
	std::cout << "Escolha uma opcao: " << std::endl;

	// Lê a opção do usuário
	if (!(std::cin >> opt)) {
		clearBuffer();
		error(INVALID_OPTION_MSG);
		return false;
	}

	// Define o tipo de item do calendário com base na opção escolhida
	TipoItemCalendario tipo = (opt == 1) ? FERIAS : (opt == 2) ? FALTA : INVALIDO;

	if (tipo == INVALIDO) {
		error(INVALID_OPTION_MSG);
		return false;
	}

	std::string dataInicio;
	clearBuffer();
	std::cout << "Insira a data (DD/MM/AAAA): ";
	std::getline(std::cin, dataInicio);

	// Valida a data
	if (!validarData(dataInicio)) {
		return false;
	}

	if (parseTipoParaString(tipo) == "Ferias" && temColaboradorComDiaFerias(colabs, parseDate(dataInicio), colab)) {
		std::cout << "Já existe um colaborador no departamento ("<< colab->getDepartamento() <<") com ferias marcadas para ("<< dataInicio <<")." << std::endl;
		std::cout << "Deseja marcar as ferias? (S/N): ";
		std::string op;
		std::getline(std::cin, op);

		if (toLower(op) == "n") return false;
	}

	// Cria o item do calendário e adiciona ao colaborador
	ItemCalendario ic(tipo, dataInicio);

	// Adiciona o item ao colaborador
	return colab->adicionarItem(ic);
}

bool marcarItemCalendario(colabList) {

	// Verifica se há colaboradores na lista
	if (colabs->size() == 0) {
		error(NO_COLABORATORS_MSG);
		return false;
	}

	// Escolhe o colaborador
	Colaborador* c = escolherColaborador(colabs);

	// Se a escolha for inválida, retorna falso
	if (c == nullptr) {
		return false;
	}

	std::cout << std::endl << "[1] Marcar" << std::endl;
	std::cout << "[2] Desmarcar" << std::endl;
	std::cout << "[0] Voltar" << std::endl;
	std::cout << "Escolha uma opcao: ";

	int opt = -1;

	// Lê a opção do usuário
	if (!(std::cin >> opt)) {
		clearBuffer();
		error(INVALID_OPTION_MSG);
		return false;
	}

	switch (opt) {
	// Marcar item
	case 1:
		return marcar(c, colabs);
		break;
	// Desmarcar item
	case 2:
		return desmarcar(c);
		break;
	// Voltar
	default:
		return false;
		break;
	}

	return false;
}

void mostrarCalendarioMensal(int mesAtual, int anoAtual, vetor<ItemCalendario>* itens) {
	vetor<ItemCalendario> items = itens->whereRange([&](ItemCalendario &cal) {
		Data d = parseDate(cal.getData());
		return d.mes == mesAtual && d.ano == anoAtual;
	});

	std::map<int, TipoItemCalendario> marcacoes;
	for (auto &item : items) {
		Data d = parseDate(item.getData());
		marcacoes[d.dia] = item.getTipo();
	}

	int ultimoDia = ultimoDiaDoMes(anoAtual, mesAtual);
	int primeiraSemana = diaSemana(1, mesAtual, anoAtual);

	std::cout << "\n------ " << nomeMes(mesAtual) << " " << anoAtual << " ------\n";
	std::cout << "Seg Ter Qua Qui Sex Sab Dom\n";

	for (int i = 0; i < primeiraSemana; i++)
		std::cout << "    ";

	for (int dia = 1; dia <= ultimoDia; dia++) {

		bool temMarcacao = marcacoes.contains(dia);

		if (temMarcacao)
			std::cout << "[" << std::setw(1) << dia << (char) marcacoes[dia] << "]";
		else
			std::cout << " " << std::setw(2) << dia << " ";

		if ((primeiraSemana + dia) % 7 == 0)
			std::cout << "\n";
	}

	std::cout << "\n";
}


void mostrarCalendarioColaborador(vetor<Colaborador>* colabs) {

    if (colabs->size() == 0) {
        error(NO_COLABORATORS_MSG);
        return;
    }

    Colaborador* c = escolherColaborador(colabs);

    if(c == nullptr) {
        return;
    }

    vetor<ItemCalendario>* itens = c->getItensCalendario();

    if (itens->size() == 0) {
        error("O colaborador ("+ c->getNome() +") nao possui itens no seu calendario");
        return;
    }

    char opt;
    int weekOffset = 0;

    const char* diaSemanaNome[7] = { "Seg", "Ter", "Qua", "Qui", "Sex", "Sab", "Dom" };

    int dia, mes, ano;
    std::cout << "Insira a data que deseja ver no calendario (dia/mes/ano): ";
    scanf_s("%d/%d/%d", &dia, &mes, &ano);

    do {

        Data inicioSemana = primeiroDiaSemana(dia, mes, ano);

        int diaAtual = inicioSemana.dia + (weekOffset * 7);
        int mesAtual = inicioSemana.mes;
        int anoAtual = inicioSemana.ano;
        int diasMesAtual = diasNoMes(mesAtual, anoAtual);

        while (diaAtual < 1) {
            mesAtual--;
            if (mesAtual < 1) {
                mesAtual = 12;
                anoAtual--;
            }
            diaAtual += diasNoMes(mesAtual, anoAtual);
        }

        while (diaAtual > diasMesAtual) {
            diaAtual -= diasMesAtual;
            mesAtual++;
            if (mesAtual > 12) {
                mesAtual = 1;
                anoAtual++;
            }
            diasMesAtual = diasNoMes(mesAtual, anoAtual);
        }

        system("cls");

        mostrarCalendarioMensal(mesAtual, anoAtual, itens);

        header("Calendario semanal de " + std::to_string(diaAtual) + "/" +
               std::to_string(mesAtual) + "/" + std::to_string(anoAtual) +
               " (semana " + (weekOffset >= 0 ? "+" : "") + std::to_string(weekOffset) + ")");

        for (int i = 0; i < 7; i++) {

            int diaIter = diaAtual + i;
            int mesIter = mesAtual;
            int anoIter = anoAtual;

            if (diaIter > diasNoMes(mesIter, anoIter)) {
                diaIter -= diasNoMes(mesIter, anoIter);
                mesIter++;
                if (mesIter > 12) {
                    mesIter = 1;
                    anoIter++;
                }
            }

            vetor<ItemCalendario> itensDia = itens->whereRange([=](ItemCalendario obj) {
                Data objData = parseDate(obj.getData());
                return objData.dia == diaIter &&
                       objData.mes == mesIter &&
                       objData.ano == anoIter;
            });

            std::cout << diaSemanaNome[i] << " (" << itensDia.size() << " item(s)) - "
                      << diaIter << " de " << nomeMes(mesIter) << "\n";

            itensDia.foreach([](int idx, ItemCalendario obj) {
                std::cout << "    > " << parseTipoParaString(obj.getTipo())
                          << " (" << obj.getData() << ")\n";
            });

            for (int j = 0; j < 15; j++) std::cout << "-";
            std::cout << "\n";
        }

        std::cout << "\nSetas </> para navegar entre semanas\n";
        std::cout << "R - Voltar semana atual\n";
        std::cout << "Q - Sair\n";

        opt = _getch();

        if (opt == 75) weekOffset--;
        else if (opt == 77) weekOffset++;
        else if (opt == 'r' || opt == 'R') weekOffset = 0;

    } while (opt != 'q' && opt != 'Q');
}


void buscarColaboradorPorIdNome(colabList) {
	header("Buscar Colaborador por ID ou Nome");

	if (colabs->size() == 0) {
		std::cout << NO_COLABORATORS_MSG << std::endl;
		return;
	}

	std::cout << "1) Buscar por ID" << std::endl;
	std::cout << "2) Buscar por Nome" << std::endl;
	std::cout << "Escolha uma opcao: " << std::endl;

	int opt = 0;
	if (!(std::cin >> opt) || opt < 1 || opt > 2) {
		std::cout << INVALID_OPTION_MSG << std::endl;
		return;
	}

	if (opt == 1) {
		Colaborador* colab = escolherColaborador(colabs);

		if (colab == nullptr) {
			error("O colaborador selecionado não existe!");
			return;
		}

		colab->mostrarDetalhesColaborador(true);

		return;
	}

	if (opt == 2) {

		std::string nome;

		listarColaboradores(colabs);
		std::cout << "Escolha o nome do colaborador: ";

		clearBuffer();

		if (!std::getline(std::cin, nome)) {
			std::cout << INVALID_OPTION_MSG << std::endl;
		}

		Colaborador* colab = colabs->where([&](Colaborador &colab) {
			return toLower(colab.getNome()) == toLower(nome);
		});

		if (colab == nullptr) {
			error("O colaborador com o nome (" + nome + ") não existe!");
			return;
		}

		colab->mostrarDetalhesColaborador(true);

		return;
	}
}

void gerarRelatorioMensal(colabList) {

    if (colabs->size() == 0) {
        error("Nao existem colaboradores para gerar relatorio.");
        return;
    }

	// Obter mês e ano atual
	auto today = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
	std::chrono::year_month_day ymd{today};
	unsigned mesAtual = static_cast<unsigned>(ymd.month());
	int anoAtual = int(ymd.year());

	namespace fs = std::filesystem;

	// Define o caminho da pasta
	fs::path dir = "relatorios";

	// Cria a pasta se não existir
	if (!fs::exists(dir)) {
		if (!fs::create_directory(dir)) {
			std::cout << "Nao foi possivel criar a pasta 'relatorios/'" << std::endl;
			return;
		}
	}

	// Cria a pasta se não existir
	if (!fs::exists("relatorios/" + nomeMes(mesAtual) + "-" + std::to_string(anoAtual))) {
		if (!fs::create_directory("relatorios/" + nomeMes(mesAtual) + "-" + std::to_string(anoAtual))) {
			std::cout << "Nao foi possivel criar a pasta 'relatorios/"<< nomeMes(mesAtual) << "-" << anoAtual <<"'" << std::endl;
			return;
		}
	}

    // Perguntar ao utilizador formato do ficheiro
    std::string formato;
    std::cout << "Escolha o formato do relatorio (txt/csv): ";
    std::cin >> formato;

    // Percorrer colaboradores
    colabs->foreach([&](int i, Colaborador c) {
    	fs::path filePath = "";

    	if (formato == "txt" || formato == "csv") {
    		filePath = dir / (nomeMes(mesAtual) + "-" + std::to_string(anoAtual)) / (c.getNome() + "_RELATORIO MENSAL." + formato);
		} else {
			error("Formato invalido.");
			return;
		}
    	std::ofstream file(filePath.string());

    	if (!file.is_open()) {
    		std::cout << "Nao foi possivel criar o ficheiro ("<< filePath.string() <<")" << std::endl;
    		return;
    	}

        vetor<ItemCalendario> ferias = c.getItensCalendario()->whereRange([&](ItemCalendario ic) {
            Data d = parseDate(ic.getData());
            return ic.getTipo() == FERIAS && d.mes == mesAtual && d.ano == anoAtual;
        });

        vetor<ItemCalendario> faltas = c.getItensCalendario()->whereRange([&](ItemCalendario ic) {
            Data d = parseDate(ic.getData());
            return ic.getTipo() == FALTA && d.mes == mesAtual && d.ano == anoAtual;
        });

        if (formato == "txt") {
        	file << "------- FERIAS -------" << std::endl;
			file << std::left << std::setw(14) << "Data"
				 << std::setw(8) << "Tipo"
				 << std::endl;
			file << std::string(36, '-') << std::endl;
			ferias.foreach([&](int i, ItemCalendario ic) {
				file << std::left << std::setw(24) << ic.getData() << std::setw(8) << "Ferias" << std::endl;
			});

        	file << std::endl << std::endl << "------- FALTAS -------" << std::endl;
			file << std::left << std::setw(14) << "Data"
				 << std::setw(8) << "Tipo"
				 << std::endl;
			file << std::string(36, '-') << std::endl;
            faltas.foreach([&](int i, ItemCalendario ic) {
				file << std::left << std::setw(24) << ic.getData() << std::setw(8) << "Falta" << std::endl;
			});
        } else if (formato == "csv") {
        	file << "Nome,Data,Tipo" << std::endl;
        	ferias.foreach([&](int i, ItemCalendario ic) {
        		file << c.getNome() << "," << ic.getData() << "," << "Ferias" << "\n";
			});
        	faltas.foreach([&](int i, ItemCalendario ic) {
        		file << c.getNome() << "," << ic.getData() << "," << "Falta" << "\n";
			});
        }

    	file.close();
    });

    success("Relatorio mensal gerado para ("+ std::to_string(colabs->size()) +") colaboradores.\n");
}

void mostrarDashboardResumido(colabList) {

	if (colabs->size() == 0) {
		std::cout << "Nao existem colaboradores para mostrar." << std::endl;
		return;
	}

	const int totalDiasFeriasAno = 22; // Total de dias de férias por colaborador

	// Cabeçalho da tabela
	std::cout << std::left << std::setw(32) << "Nome"
			  << std::setw(8) << "Ferias"
			  << std::setw(8) << "Faltas"
			  << std::setw(15) << "Dias restantes"
			  << std::endl;

	std::cout << std::string(71, '-') << std::endl;

	// Iterar sobre todos os colaboradores
	colabs->foreach([&](int i, Colaborador c) {
		int ferias = c.getItensCalendario()->whereRange([&](ItemCalendario ic) {
			return ic.getTipo() == FERIAS;
		}).size();

		int faltas = c.getItensCalendario()->whereRange([&](ItemCalendario ic) {
			return ic.getTipo() == FALTA;
		}).size();

		int diasRestantes = totalDiasFeriasAno - ferias;

		std::cout << std::left << std::setw(32) << c.getNome()
				  << std::setw(8) << ferias
				  << std::setw(8) << faltas
				  << std::setw(15) << diasRestantes
				  << std::endl;
	});

	listarColaboradores(colabs);
}