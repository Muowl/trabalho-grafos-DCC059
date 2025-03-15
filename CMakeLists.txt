# Versão mínima do CMake
cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(main)

# Definir o padrão C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Incluir diretório de headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Adicionar arquivos fonte
set(SOURCES
    main.cpp
    # Adicioar aqui outros arquivos .cpp quando forem criados
    src/leitura.cpp
    src/grafo_lista.cpp
    src/grafo_matriz.cpp
    src/grafo.cpp
    src/algoritmo_guloso.cpp
    src/algoritmo_randomizado.cpp
    src/algoritmo_relativo.cpp
    src/comunidade.cpp
)

# Criar executável
add_executable(${PROJECT_NAME} ${SOURCES})