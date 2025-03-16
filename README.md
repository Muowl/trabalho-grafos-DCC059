# Trabalho de Teoria dos Grafos - DCC059 - 2024.3

## Alunos
- Felipe Lazzarini Cunha
- Luiz Alberto Werneck Silva

## Observação
O trabalho está sendo desenvolvido e testado na ultima versão do Ubuntu (através de imagem Docker)

## Documentação
- **Dentro do diretório docs/**

## Execução do programa (De acordo com o especificado no trabalho)
- **Na pasta raiz do trabalho:**
```bash
g++ -o main.out main.cpp sanitizador.cpp -I./include/ -g -Wall -Werror
```

- **Execução com Lista:**
```bash
./ main.out -p -l grafo-testes.txt
```

- **Execução com Matriz:**
```bash
./ main.out -p -m grafo-testes.txt
```
