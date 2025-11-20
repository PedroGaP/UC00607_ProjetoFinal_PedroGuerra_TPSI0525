#include "storage.h"
#include  "lib/json.hpp"
#include "../models/colaborador/Colaborador.h"
#include "../models/calendario/ItemCalendario.h"
#include <fstream>

using json = nlohmann::json;

// Função para carregar os colaboradores do ficheiro JSON
bool loadColaboradores(colabList) {
	std::ifstream f(colabfile);

	// Se o ficheiro não existir, cria um novo ficheiro vazio
	if (!f.is_open()) {
		std::ofstream newFile(colabfile);
		if (!newFile.is_open()) {
			return false;
		}
		newFile << "[]";
		newFile.close();

		colaboradorIdIncrement = 1;
		formacaoIdIncrement = 1;
		return true;
	}

	// Lê o conteúdo do ficheiro JSON
	json data;
	try {
		f >> data;
	}
	catch (...) {
		f.close();
		return false;
	}
	f.close();

	int maxId = 1, maxFormId = 1, maxNotaId = 1;

	// Desencripta e carrega os colaboradores na lista
	for (auto& colab : data) {
		int id = colab["id"].get<int>();
		if (id > maxId) {
			maxId = id;
		}

		std::string nomeDesencriptado = decrypt(colab["nome"].get<std::string>(), secretKey);
		std::string departamentoDesencriptado = decrypt(colab["departamento"].get<std::string>(), secretKey);
		Colaborador c(colab["id"], nomeDesencriptado, departamentoDesencriptado);

		// Carrega os itens do calendário do colaborador
		if (colab.contains("itensCalendario")) {
			for (auto& item : colab["itensCalendario"]) {
				ItemCalendario ic = ItemCalendario(
					static_cast<TipoItemCalendario>(item["tipo"].get<int>()),
					item["data"].get<std::string>()
				);
				c.adicionarItem(ic);
			}
		}

		if (colab.contains("formacoes")) {
			for (auto& jf : colab["formacoes"]) {
				int fid = jf["id"].get<int>();

				if (fid > maxFormId) maxFormId = fid;
				std::string nomeForm = decrypt(jf["nome"].get<std::string>(), secretKey);

				std::string dataDec = decrypt(jf["dataConclusao"].get<std::string>(), secretKey);
				int dia, mes, ano;
				sscanf_s(dataDec.c_str(), "%d/%d/%d", &dia, &mes, &ano);
				Data d{ dia, mes, ano };

				Formacao fobj(fid, nomeForm, d);
				c.adicionarFormacao(fobj);
			}
		}

		if (colab.contains("notas")) {
			for (auto& jn : colab["notas"]) {
				int nid = jn["id"].get<int>();
				int fid = jn["formacaoId"].get<int>();

				if (nid > maxNotaId) maxFormId = nid;
				std::string texto = decrypt(jn["texto"].get<std::string>(), secretKey);

				std::string dataDec = decrypt(jn["data"].get<std::string>(), secretKey);
				int dia, mes, ano;
				sscanf_s(dataDec.c_str(), "%d/%d/%d", &dia, &mes, &ano);
				Data d{ dia, mes, ano };

				Nota nobj(nid, fid, texto, d);
				c.adicionarNota(nobj);
			}
		}

		colabs->push_back(c);
	}

	colaboradorIdIncrement = maxId;
	formacaoIdIncrement = maxFormId;
	notaIdIncrement = maxNotaId;

	return true;
}

// Função para salvar os colaboradores no ficheiro JSON
bool saveColaboradores(colabList) {
	std::ofstream f(colabfile);

	// Verifica se o ficheiro foi aberto corretamente
	if (!f.is_open()) {
		return false;
	}

	// Cria um array JSON para armazenar os dados dos colaboradores
	json data = json::array();

	// Encripta e adiciona os colaboradores ao array JSON
	colabs->foreach([&](int i, Colaborador c) {
		// Colaborador
		json colabJson;
		colabJson["nome"] = encrypt(c.getNome(), secretKey);
		colabJson["departamento"] = encrypt(c.getDepartamento(), secretKey);
		colabJson["id"] = c.getId();

		// Calendario
		vetor<ItemCalendario>* itens = c.getItensCalendario();
		json itensJson = json::array();

		itens->foreach([&](int j, ItemCalendario ic) {
			json itemJson;
			itemJson["tipo"] = static_cast<int>(ic.getTipo());
			itemJson["data"] = ic.getData();
			itensJson.push_back(itemJson);
		});
		colabJson["itensCalendario"] = itensJson;

		// Formacoes
		vetor<Formacao>* forms = c.getFormacoes();
		json formasJson = json::array();

		forms->foreach([&](int k, Formacao f) {
			json formJson;
			formJson["id"] = f.getId();
			formJson["nome"] = encrypt(f.getNome(), secretKey);

			Data d = f.getDataConclusao();
			formJson["dataConclusao"] = encrypt(d.toString(), secretKey);

			formasJson.push_back(formJson);
		});

		colabJson["formacoes"] = formasJson;

		// Notas
		vetor<Nota>* notas = c.getNotas();
		json notasJson = json::array();

		notas->foreach([&](int k, Nota n) {
			json notaJson;
			notaJson["id"] = n.getId();
			notaJson["formacaoId"] = n.getFormacaoId();
			notaJson["texto"] = encrypt(n.getTexto(), secretKey);

			Data d = n.getData();
			notaJson["data"] = encrypt(d.toString(), secretKey);

			notasJson.push_back(notaJson);
		});

		colabJson["notas"] = notasJson;

		data.push_back(colabJson);
	});

	// Escreve os dados encriptados no ficheiro JSON
	f << data.dump(4);
	f.close();
	return true;
}