#include "Colaborador.h"
#include "../../utils/utils.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <map>

#include "services/colaborador/ColaboradorService.h"

// Retorna o nome do colaborador
std::string Colaborador::getNome() {
	return nome;
}

std::string Colaborador::getDepartamento() {
	return departamento;
}

int Colaborador::getId() {
	return id;
}

// Retorna o vetor de itens do calendario do colaborador
vetor<ItemCalendario>* Colaborador::getItensCalendario() {
	return &itensCalendario;
}

vetor<Formacao>* Colaborador::getFormacoes() {
	return &formacoes;
}

vetor<Nota>* Colaborador::getNotas() {
	return &notas;
}

// Adiciona um item ao calendario do colaborador
bool Colaborador::adicionarItem(ItemCalendario& ic) {
	itensCalendario.push_back(ic);
	return true;
}

bool Colaborador::adicionarFormacao(Formacao& f) {
	formacoes.push_back(f);
	return true;
}

bool Colaborador::adicionarNota(Nota &nota) {
	notas.push_back(nota);
	return true;
}

void Colaborador::mostrarDetalhesColaborador(bool showCalendar) const {
	std::cout
		<< "ID: " << std::left << std::setw(3) << id
		<< " | Nome: " << std::left << std::setw(20) << nome
		<< " | Departamento: " << departamento
		<< std::endl;

	if (showCalendar) {
		vetor<ItemCalendario> calendario = itensCalendario;

		auto today = floor<std::chrono::days>(std::chrono::system_clock::now());
		std::chrono::year_month_day ymd{today};
		unsigned mesAtual = static_cast<unsigned>(ymd.month());
		int anoAtual = int(ymd.year());

		vetor<ItemCalendario> items = calendario.whereRange([&](ItemCalendario &cal) {
			Data d = parseDate(cal.getData());
			return d.mes == mesAtual && d.ano == anoAtual;
		});

		mostrarCalendarioMensal(mesAtual, anoAtual, &items);

	}
}
