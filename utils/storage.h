#pragma once
#include "../models/colaborador/Colaborador.h"

// Define constantes para ficheiro e chave de encriptação
#define colabfile "colaboradores.json"
#define secretKey "sistemarh"

// Função para carregar os colaboradores do ficheiro JSON
bool loadColaboradores(colabList);
// Função para salvar os colaboradores no ficheiro JSON
bool saveColaboradores(colabList);