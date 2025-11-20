#pragma once
#include <string>

//Defini��o do tipo enumerado para os tipos de itens do calend�rio	
enum TipoItemCalendario
{
	INVALIDO = 0,
	FERIAS = 'F',
	FALTA = 'X'
};

// Defini��o da estrutura ItemCalendario
struct ItemCalendario
{
private:
	TipoItemCalendario tipo;
	std::string data;
public:

	// Construtor parametrizado
	ItemCalendario(TipoItemCalendario tipo, std::string data)
		: tipo(tipo), data(data) {
	}

	// Construtor padr�o
	ItemCalendario() = default;

	// M�todos getters
	TipoItemCalendario getTipo() const;
	std::string getData() const;
};