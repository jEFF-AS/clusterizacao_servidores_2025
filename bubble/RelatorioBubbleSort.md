### AtivRelatório de Atividade de Clusterização - Bubble Sort com MPI

Bubble Sort, também conhecido como "ordenação por bolha", é um algoritmo de ordenação simples e de fácil compreensão. Ele é utilizado para organizar os elementos de um vetor, normalmente em ordem crescente (do menor para o maior) ou decrescente (do maior para o menor).

O programa exibe o vetor original, que consiste em uma sequência aleatória de números. Em seguida, apresenta-se o vetor final ordenado, com os números dispostos em ordem crescente. Posteriormente, o tempo total de execução do programa (em segundos) é apresentado.

Os dados apresentados indicam que o número de processos não tem um efeito direto e linear no tempo de execução para todos os tamanhos de vetor testados.

Vetor com 32 elementos: O tempo de execução não varia muito entre 2, 4 e 8 processos. Em certas situações, 4 processos levaram um pouco mais de tempo do que 2 processos, enquanto 8 processos mostraram resultados tanto inferiores quanto superiores aos de 2 e 4 processos. Isso indica que, para vetores muito pequenos, o custo da comunicação entre os processos pode neutralizar ou até exceder os benefícios da paralelização.

Vetor com 64 elementos: Mais uma vez, o efeito é misto. Apesar de haver uma execução com 2 processos (0.000026s) que é equivalente a uma com 4 processos (0.000023s) e com 8 processos (0.000030s), em algumas situações, o tempo com 8 processos foi superior ao de 2 ou 4 processos.

Vetor com 128 elementos: O tempo de execução tende a aumentar ou permanecer estável com o crescimento do número de processos, particularmente ao passar de 4 para 8 processos. Por exemplo, de 0,000073s para 0,000871s ao aumentar de 4 para 8 processos. Isso pode sugerir que, a partir de um determinado momento, o crescimento do número de processos pode resultar em um custo maior de comunicação ou sincronização.

Vetor com 256 elementos: Assim como nos vetores anteriores, o crescimento da quantidade de processos não assegura uma diminuição consistente no tempo. Em certos casos, o desempenho de 4 processos foi inferior ao de 2, enquanto 8 processos mostraram uma lentidão significativa em algumas execuções..

De modo geral, para o algoritmo Bubble Sort e os tamanhos de vetor e quantidades de processos testadas, não se nota uma redução de tempo significativa e consistente ao expandir o número de processos.


Um "ganho real" seria uma diminuição significativa e estável no tempo de execução à medida que o número de processos aumenta. Com base nas informações apresentadas, torna-se desafiador identificar cenários de ganho real consistente:

Em casos específicos, como no vetor de 64 elementos, uma execução com 4 processos (0,000023s) foi mais rápida do que uma com 2 processos (0,000026s), e uma com 8 processos (0,000030s) também foi mais rápida do que uma com 2 processos. Contudo, esses ganhos não se manifestam em todas as execuções e não são suficientemente abrangentes para caracterizar um "ganho real" consistente.

Para vetores maiores (128 e 256 elementos), o tempo de execução frequentemente aumentou ou permaneceu similar ao expandir a quantidade de processos, sinalizando a falta de um ganho real considerável.

As principais limitações identificadas são:

Ineficiência do Algoritmo Bubble Sort para paralelização: O Bubble Sort é um algoritmo de ordenação intrinsecamente sequencial. Como cada passagem depende dos resultados da anterior, o paralelismo fica limitado. A segmentação do vetor em sub-vetores para processos distintos e a subsequente junção (seja implícita ou explícita) podem gerar um custo de comunicação que o Bubble Sort não é capaz de compensar.

Overhead de comunicação MPI: Um fator importante é o envio e recebimento de dados entre os processos (overhead de MPI). Para vetores pequenos, esse overhead pode superar o tempo poupado com a execução paralela, levando a tempos de execução comparáveis ou até superiores.

Amostra limitada de execuções: Para determinados tamanhos de vetor e quantidades de processos, existem apenas uma ou duas execuções documentadas. Uma análise mais aprofundada se beneficiaria de várias execuções para cada cenário, possibilitando o cálculo de médias e desvios para uma avaliação mais precisa do desempenho.

Tamanho do vetor: Os vetores testados (32, 64, 128, 256) são bastante pequenos para evidenciar o potencial de paralelismo em algoritmos de ordenação. Conjuntos de dados muito maiores costumam apresentar ganhos significativos em paralelização.

Para aumentar a eficácia da ordenação em um ambiente paralelo, as seguintes estratégias podem ser levadas em conta:

Substituir o Bubble Sort por um algoritmo mais adequado para paralelização: Algoritmos como Merge Sort, Quick Sort e Radix Sort são intrinsecamente mais eficazes para paralelização. Eles podem ser desmembrados em subproblemas independentes que podem ser tratados simultaneamente, com custos de fusão (merge) ou agregação mais eficazes.

Merge Sort Paralelo: Dividir o vetor em sub-vetores, ordenar cada sub-vetor em paralelo e, em seguida, fundir os resultados de forma paralela.

Quick Sort Paralelo: A escolha do pivô e a partição podem ser feitas em paralelo.

Otimizar a comunicação MPI:

Minimizar o número de comunicações: Agrupar os dados antes de enviar ou receber.

Usar comunicação não-bloqueante: Permitir que os processos executem outras tarefas enquanto esperam pela comunicação.

Escolher a topologia de comunicação apropriada: Para grandes números de processos, uma topologia otimizada pode reduzir o tempo de comunicação.

Ampliar o tamanho do vetor: O paralelismo tende a ser mais eficiente com conjuntos de dados maiores, em que o tempo de processamento supera o tempo de comunicação.

Balanceamento de carga: Assegurar que todos os processos tenham uma quantidade equitativa de trabalho para evitar que alguns processos fiquem ociosos enquanto outros estão processando.

Uso de algoritmos de ordenação paralelos específicos: Há algoritmos de ordenação desenvolvidos especificamente para arquiteturas paralelas, como o Parallel Sort ou o Sample Sort, que podem ser mais eficientes do que apenas paralelizar algoritmos sequenciais.
