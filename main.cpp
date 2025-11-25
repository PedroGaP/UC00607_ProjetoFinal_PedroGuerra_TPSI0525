#include <iostream>
#include "utils/utils.h"
#include "models/colaborador/Colaborador.h"
#include "services/colaborador/ColaboradorService.h"
#include <fstream>
#include "lib/json.hpp"
#include "services/departamento/DepartamentoService.h"
#include "services/formacao/FormacaoService.h"
#include "services/nota/NotaService.h"
#include "utils/storage.h"

using json = nlohmann::json;

int showMenu();
bool loadColaboradores(colabList);
bool saveColaboradores(colabList);

int main(void) {

	bool isRunning = true;
	int opt = 0;

	// Cria um vetor para guardar os colaboradores do sistema
	vetor<Colaborador> colaboradores = vetor<Colaborador>();

	// Tenta carregar os colaboradores do ficheiro
	// Se falhar, exibe uma mensagem de erro e termina o programa
	if (!loadColaboradores(&colaboradores)) {
		error("Erro ao carregar dados dos colaboradores.");
		return -1;
	}

	// Loop principal do programa
	do {
		// Mostra o menu e obtém a opção do utilizador
		opt = showMenu();

		switch (opt) {
			// Tenta adicionar um colaborador
			// Exibe uma mensagem de sucesso ou erro
			case 1:
				if (adicionarColaborador(&colaboradores)) {
					success("Colaborador adicionado com sucesso.");
				}
				else {
					error("Erro ao adicionar colaborador.");
				}
				break;
			// Tenta marcar ou desmarcar um item no calendário de um colaborador
			case 2:
				if (marcarItemCalendario(&colaboradores)) {
					success("Item marcado/desmarcado com sucesso.");
				}
				else {
					error("Erro ao marcar/desmarcar item.");
				}
				break;
			// Lista todos os colaboradores
			case 3:
				listarColaboradores(&colaboradores);
				break;
			// Mostra o calendário de um colaborador específico
			case 4:
				mostrarCalendarioColaborador(&colaboradores);
				break;
			case 5:
				buscarColaboradorPorIdNome(&colaboradores);
				break;
			case 6:
				gerirFormacoesCursosColaborador(&colaboradores);
				break;
			case 7:
				gerirNotasObservacoesColaborador(&colaboradores);
				break;
			case 8:
				gerarRelatorioMensal(&colaboradores);
				break;
			case 9:
				estatisticasDepartamento(&colaboradores);
				break;
			case 10:
				mostrarDashboardResumido(&colaboradores);
				break;
			// Sai do programa, salvando os dados dos colaboradores
			// Exibe uma mensagem de erro se falhar
			case 0:
				if (!saveColaboradores(&colaboradores)) {
					error("Erro ao salvar dados dos colaboradores.");
				}
				std::cout << "A sair do programa..." << std::endl;
				isRunning = false;
				break;
			// Opção inválida
			default:
				error(INVALID_OPTION_MSG);
				break;
		}

	} while (isRunning);

}

// Função para mostrar o menu principal e obter a opção do utilizador
int showMenu() {
	std::cout << "==================================" << std::endl;
	std::cout << "|      GESTAO RH (AVANCADO)      |" << std::endl;
	std::cout << "==================================" << std::endl;
	std::cout << "1) Adicionar Colaborador" << std::endl;
	std::cout << "2) Marcar/Desmarcar Ferias e Faltas" << std::endl;
	std::cout << "3) Listar Colaboradores" << std::endl;
	std::cout << "4) Visualizar Calendario de Colaborador" << std::endl;
	std::cout << "5) Buscar Colaborador por ID ou nome" << std::endl;
	std::cout << "6) Gerir formacoes/cursos de um Colaborador" << std::endl;
	std::cout << "7) Gerir notas/observacoes de um Colaborador" << std::endl;
	std::cout << "8) Relatorios Mensais" << std::endl;
	std::cout << "9) Estatisticas por Departamento" << std::endl;
	std::cout << "10) Dashboard Resumido" << std::endl;
	std::cout << "0) Sair" << std::endl;
	std::cout << "----------------------------------" << std::endl;
	std::cout << "Escolha uma opcao: ";
	int choice;

	// Lê a opção do utilizador e trata entradas inválidas
	if (!(std::cin >> choice)) {
		clearBuffer();
		return -1;
	}

	return choice;
}


