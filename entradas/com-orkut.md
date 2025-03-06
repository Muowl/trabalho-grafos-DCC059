# Documento Explicativo: Arquivo de Comunidades e Relação com o Trabalho de Grafos

## 1. Arquivo de Comunidades: com-orkut.top5000.cmty.txt

- **Formato:**
  ```
  ID_Comunidade   ID_Usuário1   ID_Usuário2   ID_Usuário3   ...
  ```
- **Descrição:**
  Cada linha representa uma comunidade, onde o primeiro valor é o identificador da comunidade e os demais são os IDs dos usuários que pertencem a ela.

## 2. Conversão para Grafo

- **Nós:**
  Cada usuário é convertido em um nó.
- **Arestas:**
  São criadas conexões (arestas) entre usuários que compartilham a mesma comunidade.

## 3. Sobre o AGMG

- **O que é:**  
  Um algoritmo de particionamento de grafos que agrupa nós (usuários) em clusters com base em suas conexões.
  
- **Aplicação:**  
  Especialmente útil para redes grandes e esparsas, como a rede do Orkut.