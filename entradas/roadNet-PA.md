# Documento Explicativo: Arquivo roadNet-PA e Relação com o Trabalho de Grafos

## 1. Arquivo de Rede Rodoviária: roadNet-PA.txt

- **Formato:**
```
NodeId1    NodeId2
```

- **Descrição:**
O arquivo representa a rede rodoviária da Pensilvânia como um grafo não-direcionado, onde:
    - **Nós (nodes):** Interseções ou pontos terminais de estradas.
    - **Arestas (edges):** Conexões bidirecionais entre essas interseções (estradas).

**Exemplo do arquivo:**
```
0    1
0    6309
0    6353
1    0
6353    0
6353    6354
```
Aqui, o nó `0` está conectado aos nós `1`, `6309` e `6353`. Como o grafo é não-direcionado, a conexão `0 1` implica automaticamente `1 0`.

## 2. Estrutura do Grafo

- **Número total de nós:** 1.088.092 (interseções ou terminais de estrada).
- **Número total de arestas:** 3.083.796 (estradas conectando interseções, agora tratadas como bidirecionais).
- **Maior Componente Conexa (WCC - Weakly Connected Component):** 99,8% dos nós pertencem a uma única componente conexa.
- **Coeficiente de Aglomeração Médio:** 0.0352, indicando baixa formação de comunidades locais.
- **Número de Triângulos:** 95.430, onde triângulos representam pequenos ciclos de três nós interconectados.
- **Diâmetro do Grafo:** 850, representando a maior distância entre quaisquer dois nós.
- **Diâmetro Efetivo (90%):** 510, indicando que 90% dos pares de nós estão a uma distância menor ou igual a 510 arestas.

## 3. Conversão para Análise de Grafos

- **Nós:** Cada ID de nó representa uma interseção ou ponto de conexão entre estradas.
- **Arestas:** Representam as estradas, agora tratadas como conexões bidirecionais.
- **Formato de Grafo:** Como o grafo é não-direcionado, cada conexão entre nós deve ser considerada nos dois sentidos.

## 4. Árvore Geradora Mínima Generalizada (AGMG)

Uma Árvore Geradora Mínima Generalizada (AGMG) é uma estrutura de grafo que conecta todos os nós com o menor custo total possível, sem formar ciclos. No contexto do arquivo **roadNet-PA.txt**, a AGMG pode ser utilizada para:

- **Redução de custos:** Encontrar a maneira mais eficiente de conectar todas as interseções com o menor número de estradas.
- **Manutenção de infraestrutura:** Identificar as estradas mais críticas que, se removidas, desconectariam a rede.
- **Planejamento de expansão:** Auxiliar no planejamento de novas estradas para garantir a conectividade mínima necessária.

Para gerar uma AGMG a partir do grafo rodoviário da Pensilvânia, algoritmos como Prim ou Kruskal podem ser aplicados, considerando as estradas como arestas ponderadas com base em critérios como distância ou tempo de viagem.