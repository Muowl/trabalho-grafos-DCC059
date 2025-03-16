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
g++ -o main.out main.cpp src/*.cpp -I/.include/ -g -Wall -Werror
```

- **Execução com Lista (com resultado do time):**
```bash
time ./main.out -p -l grafo-teste.txt
```

- **Execução com Matriz (com resultado do time):**
```bash
time ./main.out -p -m grafo-teste.txt
```
