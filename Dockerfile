# Usar a imagem base do Ubuntu
FROM ubuntu:latest

# Atualizar o sistema e instalar dependências
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    git \
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

# Comando padrão ao iniciar o container
CMD ["./main"]