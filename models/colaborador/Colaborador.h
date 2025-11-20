#pragma once
#include <string>

#include "../formacao/Formacao.h"
#include "../../lib/vetor.h"
#include "../calendario/ItemCalendario.h"
#include "models/nota/Nota.h"

// Define um alias para o tipo vetor de colaboradores
#define colabList vetor<Colaborador> *colabs
inline int colaboradorIdIncrement = 1;

// Estrutura que representa um colaborador
struct Colaborador {
private:
	int id;
	std::string nome;
	std::string departamento;
	vetor<ItemCalendario> itensCalendario = vetor<ItemCalendario>();
	vetor<Formacao> formacoes = vetor<Formacao>();
	vetor<Nota> notas = vetor<Nota>();

public:
	// Construtor parametrizado
	Colaborador(int id, std::string nome, std::string departamento) : id(id), nome(nome), departamento(departamento) {}
	// Construtor padr�o
	Colaborador() = default;

	std::string getNome();
	std::string getDepartamento();
	int getId();
	vetor<ItemCalendario> *getItensCalendario();
	vetor<Formacao> *getFormacoes();
	vetor<Nota> *getNotas();

	bool adicionarItem(ItemCalendario &ic);
	bool removerItem();
	bool adicionarFormacao(Formacao &formacao);
	bool adicionarNota(Nota &nota);
	void mostrarDetalhesColaborador(bool showCalendar = false) const;
};

Colaborador* escolherColaborador(colabList);
bool adicionarColaborador(colabList);
void listarColaboradores(colabList);
bool marcarItemCalendario(colabList);
void mostrarCalendarioColaborador(colabList);