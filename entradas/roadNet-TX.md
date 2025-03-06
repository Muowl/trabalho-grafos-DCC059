# Documento Explicativo: Arquivo roadNet-TX e Relação com o Trabalho de Grafos

## 1. Arquivo de Rede Rodoviária: roadNet-TX.txt

- **Formato:**
```
FromNodeId    ToNodeId
```

- **Descrição:**
O arquivo representa a rede rodoviária do Texas como um grafo não-direcionado, onde:
    - **Nós (nodes):** Interseções ou pontos terminais de estradas.
    - **Arestas (edges):** Conexões entre essas interseções (estradas).

**Exemplo do arquivo:**
```
0    1
0    2
0    29
1    0
1    23
1    32
```
Aqui, o nó `0` está conectado aos nós `1`, `2` e `29`. O nó `1` está conectado a `0`, `23` e `32`. Como o grafo é não-direcionado, a conexão `0 1` implica automaticamente `1 0`, mesmo que possa aparecer uma única vez no arquivo.

## 2. Estrutura do Grafo

- **Número total de nós:** 1.379.917 (interseções ou terminais de estrada).
- **Número total de arestas:** 3.843.320 (estradas conectando interseções).
- **Maior Componente Conexa (WCC - Weakly Connected Component):** 99,6% dos nós pertencem a uma única componente conexa.
- **Maior Componente Fortemente Conexa (SCC - Strongly Connected Component):** Idêntica à WCC, pois o grafo é não-direcionado.
- **Coeficiente de Aglomeração Médio:** Valor não especificado, mas esperado ser similar a outras redes rodoviárias.
- **Número de Triângulos:** Não especificado, mas indica ciclos pequenos dentro do grafo.
- **Diâmetro do Grafo:** Não especificado, mas representa a maior distância entre quaisquer dois nós.
- **Diâmetro Efetivo (90%):** Representa a distância máxima para 90% dos pares de nós dentro do grafo.

## 3. Conversão para Análise de Grafos

- **Nós:** Cada ID de nó representa uma interseção ou ponto de conexão entre estradas.
- **Arestas:** Representam as estradas e podem ser utilizadas para modelagem de tráfego.
- **Formato de Grafo:** Como o grafo é não-direcionado, cada conexão deve ser considerada bidirecional, mesmo que esteja escrita apenas uma vez.

## 4. Árvore Geradora Mínima Generalizada (AGMG)

Uma Árvore Geradora Mínima Generalizada (AGMG) é uma estrutura de grafo que conecta todos os nós com o menor custo total possível, sem formar ciclos. No contexto do arquivo **roadNet-TX.txt**, a AGMG pode ser utilizada para:

- **Redução de custos:** Encontrar a maneira mais eficiente de conectar todas as interseções com o menor número de estradas.
- **Manutenção de infraestrutura:** Identificar as estradas mais críticas que, se removidas, desconectariam a rede.
- **Planejamento de expansão:** Auxiliar no planejamento de novas estradas para garantir a conectividade mínima necessária.

Para gerar uma AGMG a partir do grafo rodoviário do Texas, algoritmos como Prim ou Kruskal podem ser aplicados, considerando as estradas como arestas ponderadas com base em critérios como distância ou tempo de viagem.