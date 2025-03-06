# Documento Explicativo: Arquivo com-LiveJournal e Relação com o Trabalho de Grafos

## 1. Arquivo de Rede Social: com-LiveJournal

- **Formato:**
```
Comunidades representadas como grupos de IDs de nós
```

- **Descrição:**
O arquivo contém a rede social do LiveJournal, onde:
    - **Nós (nodes):** Representam usuários individuais da rede.
    - **Arestas (edges):** Representam relações de amizade entre usuários.
    - **Comunidades:** Grupos de usuários formados dentro da plataforma, baseados na definição do próprio LiveJournal.

**Exemplo do arquivo:**
```
1852932    1996272    3314431  
1173600    4016759    4016760  
2564455    2932019    3475754  
```
Cada linha representa uma comunidade, contendo IDs de usuários que pertencem a ela.

## 2. Estrutura do Grafo

- **Número total de nós:** 3.997.962 (usuários)
- **Número total de arestas:** 34.681.189 (amizades)
- **Número total de comunidades:** 287.512
- **Maior Componente Conexa (WCC - Weakly Connected Component):** 100% dos nós pertencem a uma única WCC, indicando que todos os usuários estão conectados indiretamente.
- **Maior Componente Fortemente Conexa (SCC - Strongly Connected Component):** 100% dos nós pertencem a uma única SCC, indicando um alto grau de interconectividade.
- **Coeficiente de Aglomeração Médio:** 0.2843, sugerindo que há uma tendência de formação de grupos de amigos interligados.
- **Número de Triângulos:** 177.820.130, representando laços de amizade compartilhados entre três usuários.
- **Diâmetro do Grafo:** 17, indicando a maior distância entre quaisquer dois usuários na rede.
- **Diâmetro Efetivo (90%):** 6.5, mostrando que 90% das conexões entre pares de usuários podem ser percorridas em até 6.5 arestas.

## 3. Conversão para Análise de Grafos

- **Nós:** Cada ID de nó representa um usuário da rede social LiveJournal.
- **Arestas:** Representam relações de amizade entre usuários, indicando conexões bidirecionais.
- **Formato de Grafo:** O grafo é não-direcionado, ou seja, se um usuário A tem amizade com um usuário B, a conexão é mútua e deve ser considerada nos dois sentidos.
- **Comunidades:** Conjuntos de nós interconectados que compartilham interesses comuns ou interações frequentes.
- **Topologia:** O grafo apresenta um alto número de triângulos, evidenciando agrupamentos e laços sociais fortes entre usuários.

## 4. Comunidades e Análise de Grafos

- **Comunidades:** Representam grupos de usuários com interesses ou conexões sociais comuns.
- **Critério de Inclusão:** Apenas comunidades com pelo menos 3 membros foram consideradas no conjunto de dados.
- **Top 5.000 Comunidades:** As melhores comunidades foram selecionadas com base em métricas de qualidade descritas em pesquisas acadêmicas.

## 5. Árvore Geradora Mínima Generalizada (AGMG)

Uma Árvore Geradora Mínima Generalizada (AGMG) é uma estrutura de grafo que conecta todos os nós com o menor custo total possível, sem formar ciclos. No contexto do arquivo **com-LiveJournal**, a AGMG pode ser utilizada para:

- **Redução de custos:** Encontrar a maneira mais eficiente de conectar todos os usuários com o menor número de relações de amizade.
- **Manutenção de infraestrutura:** Identificar as conexões mais críticas que, se removidas, desconectariam a rede.
- **Planejamento de expansão:** Auxiliar no planejamento de novas conexões para garantir a conectividade mínima necessária.

Para gerar uma AGMG a partir do grafo social do LiveJournal, algoritmos como Prim ou Kruskal podem ser aplicados, considerando as relações de amizade como arestas ponderadas com base em critérios como frequência de interação ou força da conexão.