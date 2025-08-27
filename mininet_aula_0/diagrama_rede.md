## Diagrama de topologia de árvore usando a sintaxe do Mermaid.

graph TD
    %% Switch raiz
    S1[Switch 1]

    %% Switches do nível 2
    S2[Switch 2]
    S3[Switch 3]

    %% Hosts
    H1[Host 1]
    H2[Host 2]
    H3[Host 3]
    H4[Host 4]

    %% Conexões
    S1 --> S2
    S1 --> S3

    S2 --> H1
    S2 --> H2

    S3 --> H3
    S3 --> H4

Como funciona:

S1 → switch raiz

S2 e S3 → switches do nível 2

H1 a H4 → hosts conectados aos switches do nível 2

--> → indica conexão/links
