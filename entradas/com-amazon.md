# Documento Explicativo: Arquivo de Comunidades e Relação com o Trabalho de Grafos

## 1. Arquivo de Comunidades: com-amazon.top5000.cmty.txt

- **Formato:**
```
ID_Usuário1   ID_Usuário2   ID_Usuário3   ...
```

- **Descrição:**
Cada linha representa uma comunidade distinta na rede Amazon. Os valores em cada linha são os IDs dos nós (usuários ou produtos) que pertencem a essa comunidade. Diferentemente de outros arquivos como o do Orkut, não há um identificador explícito para a comunidade no início da linha; cada linha é implicitamente uma comunidade diferente, e os números listados são os membros dessa comunidade.

**Exemplo do arquivo:**
```
164985 225214 232761
105089 282815 331787
167906 288975 421279 442612 451312
```

Aqui, a primeira linha representa uma comunidade com os nós `164985`, `225214` e `232761`, a segunda linha outra comunidade com `105089`, `282815` e `331787`, e assim por diante.

- **Observação:**
O arquivo contém repetições de algumas linhas (como `55727 78359 170327 335007` que aparece várias vezes), o que pode indicar redundâncias nos dados ou comunidades idênticas detectadas múltiplas vezes. Para o trabalho de grafos, essas duplicatas podem ser tratadas como uma única comunidade.

## 2. Conversão para Grafo

- **Nós:**
Cada ID único presente no arquivo é convertido em um nó do grafo. Esses nós podem representar usuários ou produtos da rede Amazon, dependendo do contexto da base de dados.

- **Arestas:**
As arestas são criadas entre os nós que aparecem na mesma linha, ou seja, dentro da mesma comunidade. Isso significa que cada comunidade forma um subgrafo completo (um clique), onde todos os nós estão conectados entre si. Por exemplo, na linha `164985 225214 232761`, arestas seriam criadas entre `164985-225214`, `164985-232761` e `225214-232761`.


## 3. Sobre o AGMG

- **O que é:**
A Árvore Geradora Mínima Generalizada (AGMG) é uma extensão da Árvore Geradora Mínima (MST, do inglês *Minimum Spanning Tree*). Diferentemente da MST tradicional, que conecta todos os nós de um grafo com o menor custo total, a AGMG trabalha com um grafo particionado em clusters (neste caso, as comunidades). O objetivo é encontrar uma árvore de custo mínimo que conecte pelo menos um nó de cada cluster.

- **Aplicação:**
No contexto da rede Amazon, as comunidades definidas no arquivo `com-amazon.top5000.cmty.txt` podem ser usadas como clusters para a AGMG. Cada linha do arquivo representa um grupo de nós relacionados (por exemplo, produtos frequentemente comprados juntos ou usuários com interesses similares). O algoritmo de AGMG pode ser aplicado para encontrar uma estrutura eficiente que conecte todas essas comunidades com o menor custo total, o que pode ser útil para tarefas como:
- **Análise de Rede:** Reduzir a complexidade de uma rede grande ao conectar comunidades de forma otimizada.

- **Ideias de como aplicaremos no trabalho:**
1. **Definir os Clusters:** Cada linha do arquivo como um cluster de nós.
2. **Construir o Grafo Base:** Um grafo onde os nós dentro de cada comunidade são totalmente conectados (cliques). Se disponível, combine isso com uma rede de arestas pré-existente (como co-compras) para definir custos entre nós de diferentes comunidades.
3. **Aplicar AGMG:** Executar o algoritmo de AGMG para encontrar uma árvore que conecte pelo menos um nó de cada comunidade, minimizando o custo total das arestas. Isso pode envolver a atribuição de pesos às arestas (por exemplo, baseados em frequência de co-compra ou similaridade).