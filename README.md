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