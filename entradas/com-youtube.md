# Documento Explicativo: Arquivo de Comunidades e Relação com o Trabalho de Grafos

## 1. Arquivo de Comunidades: com-youtube.top5000.cmty.txt

- **Formato:**
    ```
    ID_Nó1   ID_Nó2   ID_Nó3   ...
    ```
- **Descrição:**
    Cada linha representa uma comunidade ou um conjunto de nós que pertencem a uma comunidade específica. Os números são os identificadores dos nós (usuários) que pertencem a uma determinada comunidade.

    Por exemplo, a primeira linha:
    ```
    72   165   517   875   1024   1742   2059   2252   2429   3622   5036   5080   5088   5703   5873   6723   6752   8459   8752   8885   9553   10243   10763   13040   13841   15059   15231   15310   16100   19170   19573   20572   26797   27414   28080   28084   28755   40404   43824   48404   48423   48431   61005   83295   103527   108236   139888   156355   156534   175210   180201   181647   181664   181665   183004   203349   224411   279007   282577   287218   319813   458127   526519   645203
    ```
    Isso indica que os nós com IDs 72, 165, 517, ..., 645203 pertencem à mesma comunidade.

## 2. Conversão para Grafo

    - **Nós:**
        Cada usuário é convertido em um nó.
    - **Arestas:**
        São criadas conexões (arestas) entre usuários que compartilham a mesma comunidade.

## 3. Sobre o AGMG

- **O que é:**  
  Um algoritmo de particionamento de grafos que agrupa nós (usuários) em clusters com base em suas conexões.
  
- **Aplicação:**  
  Especialmente útil para redes grandes e esparsas, como a rede do Youtube.