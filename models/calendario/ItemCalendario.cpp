#include "ItemCalendario.h"

// Retorna o tipo do item do calend�rio
TipoItemCalendario ItemCalendario::getTipo() const {
	return tipo;
}

// Retorna a data do item do calend�rio
std::string ItemCalendario::getData() const {
	return data;
}