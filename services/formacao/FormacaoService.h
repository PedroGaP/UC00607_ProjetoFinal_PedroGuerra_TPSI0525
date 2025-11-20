#pragma once
#include "../../models/colaborador/Colaborador.h"

// Formacoes
void gerirFormacoesCursosColaborador(colabList);
Formacao* escolherFormacao(Colaborador* colab);
void listarFormacoes(Colaborador* colab);
void adicionarFormacao(Colaborador* colab);
void editarFormacao(Colaborador* colab);
void removerFormacao(Colaborador* colab);