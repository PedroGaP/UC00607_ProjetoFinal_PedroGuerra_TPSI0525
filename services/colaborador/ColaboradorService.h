#pragma once
#include "../../models/colaborador/Colaborador.h"

// Colaboradores
Colaborador* escolherColaborador(colabList);
bool adicionarColaborador(colabList);
void listarColaboradores(colabList);
bool desmarcar(Colaborador* colab);
bool marcar(Colaborador* colab, colabList);
bool marcarItemCalendario(colabList);
void mostrarCalendarioMensal(int mesAtual, int anoAtual, vetor<ItemCalendario>* itens);
void mostrarCalendarioColaborador(colabList);
void buscarColaboradorPorIdNome(colabList);
void gerarRelatorioMensal(colabList);
void mostrarDashboardResumido(colabList);