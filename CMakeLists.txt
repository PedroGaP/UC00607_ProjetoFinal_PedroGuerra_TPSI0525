cmake_minimum_required(VERSION 3.15)

project(GestaoRH LANGUAGES CXX)

# Define o standard C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

# Adiciona todos os ficheiros .cpp
set(SOURCES
        ./main.cpp
        ./models/colaborador/Colaborador.cpp
        ./models/calendario/ItemCalendario.cpp
        ./lib/json.hpp
        ./utils/utils.cpp
        ./services/colaborador/ColaboradorService.cpp
        ./models/formacao/Formacao.cpp
        ./models/formacao/Formacao.cpp
        ./utils/storage.cpp
        ./services/formacao/FormacaoService.cpp
        ./models/nota/Nota.cpp
        ./services/nota/NotaService.cpp
        ./services/departamento/DepartamentoService.cpp
)

set(HEADERS
        ./models/colaborador/Colaborador.h
        ./models/calendario/ItemCalendario.h
        ./lib/json.hpp
        ./utils/utils.h
        ./lib/vetor.h
        ./services/colaborador/ColaboradorService.h
        ./models/formacao/Formacao.cpp
        ./models/formacao/Formacao.h
        ./utils/storage.cpp
        ./utils/storage.h
        ./services/formacao/FormacaoService.h
        ./models/nota/Nota.cpp
        ./models/nota/Nota.h
        ./services/nota/NotaService.h
        ./services/nota/NotaService.cpp
        ./services/departamento/DepartamentoService.h
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

