cmake_minimum_required(VERSION 3.15)

project(GestaoRH LANGUAGES CXX)

# Define o standard C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adiciona todos os ficheiros .cpp
set(SOURCES
        main.cpp
        models/colaborador/Colaborador.cpp
        models/calendario/ItemCalendario.cpp
        ./utils/utils.cpp
)

# Adiciona ficheiros .h (não obrigatório, mas útil no IDE)
set(HEADERS
        models/colaborador/Colaborador.h
        models/calendario/ItemCalendario.h
        ./lib/json.hpp
        ./utils/utils.h
        # Este caminho é extremamente longo e estranho, podes ajustar
        ./lib/vetor.h
        services/colaborador/ColaboradorService.cpp
        services/colaborador/ColaboradorService.h
        models/formacao/Formacao.cpp
        models/formacao/Formacao.h
        utils/storage.cpp
        utils/storage.h
        services/formacao/FormacaoService.cpp
        services/formacao/FormacaoService.h
        models/nota/Nota.cpp
        models/nota/Nota.h
        services/nota/NotaService.h
        services/nota/NotaService.cpp
        services/Departamento/DepartamentoService.h
        services/Departamento/DepartamentoService.cpp
)

# Cria o executável
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Inclui diretórios para os headers
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Opções para Debug/Release (equivalente ao .vcxproj)
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3 /permissive- /Zc:preprocessor /EHsc)
endif()

