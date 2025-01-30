# Trabalho de Teoria dos Grafos - DCC059 - 2024.3

## Alunos
- Felipe Lazzarini Cunha
- Luiz Alberto Werneck Silva

## Observação
O trabalho está sendo desenvolvido e testado na ultima versão do Ubuntu (através de imagem Docker)

## Documentação
- **Dentro do diretório docs/**

## Execução do programa (Docker)
1. **Verificar se Docker está instalado na máquina:**
    ```bash
    docker --version
    ```
2. **Na pasta raiz do trabalho "/trabalho-grafos-DCC059/" digite os comandos:**
    ```bash
    docker build -t trabalho_grafos .
    ```
    ```bash
    docker run --rm -it trabalho_grafos
    ```
- **Para acessar o bash do container:**
    ```bash
    docker run --rm -it trabalho_grafos /bin/bash
    ```
    
## Execução do programa (CMake - Linux)
1. **Criar o diretório de build**:
    ```bash
    mkdir -p build
    ```

2. **Limpar o conteúdo do diretório de build (se existir) e entrar no diretório**:
    ```bash
    rm -rf build/* && cd build
    ```

3. **Gerar os arquivos de construção**:
    ```bash
    cmake ..
    ```

4. **Compilar o projeto**:
    ```bash
    make
    ```
5. **Executar o programa**:
    ```bash
    ./main
    ```