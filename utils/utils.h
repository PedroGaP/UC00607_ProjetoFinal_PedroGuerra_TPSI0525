#pragma once
#include "../models/calendario/ItemCalendario.h"
#include <string>

// Cores ANSI para sa�da no terminal
const std::string RESET = "\033[0m";
const std::string WHITE = "\033[37m";
const std::string RED_BG = "\033[41m";
const std::string GREEN_BG = "\033[42m";

#define INVALID_OPTION_MSG "Erro: Digite uma opcao valida!"
#define NO_COLABORATORS_MSG "Erro: Nao existem colaboradores no sistema!"

// Estrutura de data
struct Data {
    int dia;
    int mes;
    int ano;

    bool compare(Data other);

    std::string toString() {
        return std::to_string(dia) + "/" +  std::to_string(mes) + "/" +  std::to_string(ano);
    }
};

// Fun��es utilit�rias
std::string encrypt(const std::string& text, const std::string& key);
std::string decrypt(const std::string& text, const std::string& key);
void clearBuffer();
void header(std::string text);
void error(std::string text);
void success(std::string text);
int diaSemana(int dia, int mes, int ano);
int diasNoMes(int mes, int ano);
std::string nomeMes(int mes);
std::string parseTipoParaString(TipoItemCalendario tipo);
int diaSemanaSegundaDomingo(int dia, int mes, int ano);
Data primeiroDiaSemana(int dia, int mes, int ano);
int ultimoDiaDoMes(int ano, int mes);
bool validarData(std::string data);
Data parseDate(std::string data);
std::string toLower(std::string s);

