# Documento Explicativo: Arquivo roadNet-PA e Relação com o Trabalho de Grafos

## 1. Arquivo de Rede Rodoviária: roadNet-PA.txt

- **Formato:**
```
FromNodeId    ToNodeId
```

- **Descrição:**
O arquivo representa a rede rodoviária da Pensilvânia como um grafo direcionado, onde:
    - **Nós (nodes):** Interseções ou pontos terminais de estradas.
    - **Arestas (edges):** Conexões entre essas interseções (estradas), respeitando a direcionalidade.

**Exemplo do arquivo:**
```
0    1
0    6309
0    6353
1    0
6353    0
6353    6354
```
Aqui, o nó `0` está conectado aos nós `1`, `6309` e `6353`. Como o grafo é direcionado, a conexão `0 1` não implica automaticamente `1 0`, a menos que seja explicitamente definida.

## 2. Estrutura do Grafo

- **Número total de nós:** 1.088.092 (interseções ou terminais de estrada).
- **Número total de arestas:** 3.083.796 (estradas conectando interseções).
- **Maior Componente Conexa (WCC - Weakly Connected Component):** 99,8% dos nós pertencem a uma única componente conexa quando tratado como um grafo não-direcionado.
- **Maior Componente Fortemente Conexa (SCC - Strongly Connected Component):** 67% dos nós pertencem a uma única SCC, indicando que uma grande parte da rede tem conectividade bidirecional.
- **Coeficiente de Aglomeração Médio:** 0.0352, indicando baixa formação de comunidades locais.
- **Número de Triângulos:** 95.430, onde triângulos representam pequenos ciclos de três nós interconectados.
- **Diâmetro do Grafo:** 850, representando a maior distância entre quaisquer dois nós.
- **Diâmetro Efetivo (90%):** 510, indicando que 90% dos pares de nós estão a uma distância menor ou igual a 510 arestas.

## 3. Conversão para Análise de Grafos

- **Nós:** Cada ID de nó representa uma interseção ou ponto de conexão entre estradas.
- **Arestas:** Representam as estradas, e sua direcionalidade é crucial para modelagem de tráfego.
- **Formato de Grafo:** Como o grafo é direcionado, é necessário considerar o sentido das conexões ao modelar trajetos e rotas.

## 4. Árvore Geradora Mínima Generalizada (AGMG)

Uma Árvore Geradora Mínima Generalizada (AGMG) é uma estrutura de grafo que conecta todos os nós com o menor custo total possível, sem formar ciclos. No contexto do arquivo **roadNet-PA.txt**, a AGMG pode ser utilizada para:

- **Redução de custos:** Encontrar a maneira mais eficiente de conectar todas as interseções com o menor número de estradas.
- **Manutenção de infraestrutura:** Identificar as estradas mais críticas que, se removidas, desconectariam a rede.
- **Planejamento de expansão:** Auxiliar no planejamento de novas estradas para garantir a conectividade mínima necessária.

Para gerar uma AGMG a partir do grafo rodoviário da Pensilvânia, algoritmos como Prim ou Kruskal podem ser aplicados, considerando as estradas como arestas ponderadas com base em critérios como distância ou tempo de viagem.