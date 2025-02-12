# Usar a imagem base do Ubuntu
FROM ubuntu:latest

# Atualizar o sistema e instalar dependências
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    git \
    valgrind \
    && apt-get clean

# Definir o diretório de trabalho
WORKDIR /app

# Copiar o conteúdo do projeto para o diretório de trabalho
COPY . /app

# Criar o diretório de build
RUN mkdir -p build

# Entrar no diretório de build e compilar o projeto
WORKDIR /app/build
RUN rm -rf CMakeCache.txt CMakeFiles
RUN cmake .. && make

# Comando padrão ao iniciar o container, a parte do \u001b[33m e \u001b[0m é para colorir o texto de log do Valgrind em amarelo
# Alterar o -l para grafo_lista ou -m para grafo_matriz
CMD ["sh", "-c", "valgrind --leak-check=full --log-file=/app/valgrind-report.txt ./main -d -l ../entradas/grafo.txt && echo -e '\u001b[33m' && cat /app/valgrind-report.txt && echo -e '\u001b[0m'"]