# Documento Explicativo: Arquivo de Grafos "com-Friendster.txt"

## 1. Arquivo de Grafos: com-Friendster.txt

- **Formato:**
  - Friendster community graph.
  - Total de Nós: 685.230  
  - Total de Arestas: 7.600.595  
  - Cada linha segue o formato:
  FromNodeId    ToNodeId
  
- **Descrição:**  
O arquivo contém a representação de um **grafo direcionado** que modela uma rede de usuários da plataforma Friendster.  
- **Nós:** Cada usuário é identificado por um número inteiro único.  
- **Arestas:** Cada linha indica uma ligação de um nó de origem para um nó de destino, representando uma conexão entre os usuários.  
**Exemplo:**  
- `1    2`: o usuário 1 possui uma conexão com o usuário 2.  
- `254913    255378`: representa uma conexão do usuário 254913 para o 255378.

---

## Breve Resumo do Arquivo: com-friendster.top5000.cmty.txt
- O arquivo contém as 5.000 comunidades de maior qualidade da rede Friendster.

### Contexto da Plataforma Friendster
- Friendster funcionava inicialmente como uma rede social, permitindo que usuários criassem grupos (ground-truth communities).
- Posteriormente, transformou-se em um site de jogos, mas a base do grafo permanece valiosa para análise.

### Estrutura do Arquivo
- Cada linha lista os IDs dos usuários pertencentes a determinada comunidade.
- Apenas grupos com 3 ou mais usuários foram incluídos, refletindo o maior componente conectado.

### Possíveis Aplicações em Grafos
- Estudos de comunidades em redes sociais.
- Implementação de algoritmos como AGMG para conectar pelo menos um nó de cada comunidade.

### Descrição do Site da Base
- Dados obtidos via The Web Archive Project, onde o grafo completo está disponível.
- Conexões (arestas) não estão incluídas no arquivo .cmty, mas podem ser combinadas ao grafo completo.

---

## 2. Estrutura do Grafo

- **Nós:** Representam usuários da plataforma Friendster.  
- **Arestas:** Definidas conforme a direção (FromNodeId → ToNodeId).  
- **Observações:**  
- O grafo é direcionado.  
- Cada par não ordenado de nós é registrado apenas uma vez, evitando duplicações.

---

## 3. Aplicação em Trabalhos de Grafos

**Ideias para possíveis aplicações para nosso projeto:**  
- **Estudos de Redes:** Analisar a estrutura e interconectividade dos usuários da plataforma Friendster.  
- **Detecção de Comunidades:** Aplicar algoritmos para identificar clusters de usuários.  
- **Aplicação de AGMG:** Adaptar o grafo (por exemplo, convertendo-o em não direcionado) para construir uma Árvore Geradora Mínima Generalizada.  

**Eventuais passos que podemos aplicar:**  
1. Converter o grafo direcionado em não direcionado (opcional).  
2. Definir clusters usando técnicas de detecção de comunidades.  
3. Atribuir pesos às arestas (por exemplo, peso uniforme).  
4. Executar o algoritmo AGMG para conectar os clusters de forma otimizada.

---

## 4. Considerações Finais

- **Escala e Complexidade:** Com 685.230 nós e 7.600.595 arestas, o grafo é uma base robusta para análises em redes reais.  
- **Flexibilidade:** Pode ser combinado com dados adicionais para enriquecer a análise e validar resultados em contextos práticos.