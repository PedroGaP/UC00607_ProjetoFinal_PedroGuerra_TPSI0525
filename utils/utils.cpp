#include "utils.h"

#include <algorithm>
#include <iostream>
#include <limits>

// Implementa��o do m�todo compare da struct Data
bool Data::compare(Data other) {
    return (dia == other.dia && mes == other.mes && ano == other.ano);
}

// Fun��es de criptografia simples (XOR)
std::string encrypt(const std::string& text, const std::string& key) {
    std::string result = text;
    for (size_t i = 0; i < text.size(); ++i)
        result[i] = text[i] ^ key[i % key.size()];
    return result;
}

// A fun��o de descriptografia � id�ntica � de criptografia
std::string decrypt(const std::string& text, const std::string& key) {
    return encrypt(text, key);
}

// Fun��o para limpar o buffer de entrada   
void clearBuffer() {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void header(std::string text) {
    std::cout << std::endl << "--- " << text << " ---" << std::endl;
}

void error(std::string text) {
    std::cout << WHITE << RED_BG << "Erro: " << text << RESET << std::endl;
}

void success(std::string text) {
    std::cout << WHITE << GREEN_BG << "Sucesso: " << text << RESET <<  std::endl;
}

// Fun��es relacionadas a datas
int diaSemana(int dia, int mes, int ano) {
    if (mes < 3) { mes += 12; ano--; }
    int k = ano % 100;
    int j = ano / 100;
    int h = (dia + 13 * (mes + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
    int d = (h + 6) % 7; // 0=Dom, 1=Seg, ..., 6=Sab

    if (d == 0) return 6;
    return d - 1;
}

int diasNoMes(int mes, int ano) {
    switch (mes) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12: return 31;
    case 4: case 6: case 9: case 11: return 30;
    case 2: return ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) ? 29 : 28;
    default: return 30;
    }
}
std::string nomeMes(int mes) {
    const std::string meses[] = { "Janeiro","Fevereiro","Mar�o","Abril","Maio","Junho",
                                 "Julho","Agosto","Setembro","Outubro","Novembro","Dezembro" };
    return meses[mes - 1];
}

// Ajusta o dia da semana para que segunda-feira seja 0 e domingo seja 6
int diaSemanaSegundaDomingo(int dia, int mes, int ano) {
    int d = diaSemana(dia, mes, ano);
    return (d == 0) ? 6 : d - 1;
}

// Retorna a data do primeiro dia da semana (segunda-feira) para a data fornecida
Data primeiroDiaSemana(int dia, int mes, int ano) {
    int d = diaSemanaSegundaDomingo(dia, mes, ano);
    dia -= d;

    if (dia < 1) {
        mes--;
        if (mes < 1) {
            mes = 12;
            ano--;
        }
        dia += diasNoMes(mes, ano);
    }

    return { dia, mes, ano };
}

int ultimoDiaDoMes(int ano, int mes) {
    static const int dias[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    if (mes != 2) return dias[mes - 1];

    bool bissexto = (ano % 400 == 0) || (ano % 4 == 0 && ano % 100 != 0);
    return bissexto ? 29 : 28;
}

// Fun��o para converter TipoItemCalendario para string
std::string parseTipoParaString(TipoItemCalendario tipo) {
    switch (tipo) {
        case FERIAS:
            return "Ferias";
        case FALTA:
            return "Falta";
        default:
            return "Desconhecido";
    }
}

// Fun��o para validar uma data no formato "dd/mm/yyyy"
bool validarData(std::string data) {
    int day, month, year;
	// Verifica se a data est� no formato correto e se � v�lida
    if (sscanf_s(data.c_str(), "%d/%d/%d", &day, &month, &year) != 3 ||
        day < 1 || day > diasNoMes(month, year) || month < 1 || month > 12 || year < 1900) {
        std::cout << RED_BG << WHITE << "Data invalida." << RESET << std::endl;
        return false;
    }

	// Verifica se a data cai em um fim de semana
    int diaSemanaD = diaSemana(day, month, year);

	// 0 = Domingo, 6 = S�bado
    if (diaSemanaD == 0 || diaSemanaD == 6) {
        std::cout << RED_BG << WHITE << "Nao e possivel marcar itens de calendario em fins de semana ou feriados." << RESET << std::endl;
        return false;
    }

    return true;
}

// Fun��o para converter uma string no formato "dd/mm/yyyy" para a struct Data
Data parseDate(std::string data) {
    Data d{};
    sscanf_s(data.c_str(), "%d/%d/%d", &d.dia, &d.mes, &d.ano);
    return d;
}

std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return s;
}

