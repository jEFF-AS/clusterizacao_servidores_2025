# 📄 Relatório: Jantar dos Filósofos com MPI
## 🎯 Objetivo da Checagem
Nesta checagem, trabalhamos com o clássico Problema do Jantar dos Filósofos, utilizando MPI (Message Passing Interface).
O objetivo foi:
* Modelar cada filósofo como um processo MPI.
* Implementar sincronização e exclusão mútua entre processos.
* Identificar e corrigir um bug proposital no código fornecido, que poderia causar deadlock.
* Analisar as saídas do programa, observando o comportamento dos filósofos e detectando eventuais bloqueios.


## Código Base Utilizado
O código fornecido apresentava o seguinte comportamento:
* Cada filósofo alterna entre pensar, ficar com fome e comer.
* O estado é enviado aos vizinhos utilizando MPI_Send, e o processo aguarda o estado dos vizinhos utilizando MPI_Recv.
* Após comer, o filósofo volta ao estado pensando.


### Execução
```bash
mpirun --oversubscribe -np 5 ./filosofos > saida.txt
```
### Código original (com bug proposital)
```
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define N 5
enum { THINKING, HUNGRY, EATING };

int estado;

int esquerda(int rank) { return (rank + N - 1) % N; }
int direita(int rank) { return (rank + 1) % N; }

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != N) {
        if (rank == 0) printf("Execute com exatamente %d processos.\n", N);
        MPI_Finalize();
        return 0;
    }

    srand(time(NULL) + rank);
    estado = THINKING;

    for (int step = 0; step < 10; step++) {

        printf("Filósofo %d pensando.\n", rank);
        sleep(rand() % 2);

        estado = HUNGRY;
        printf("Filósofo %d com fome.\n", rank);

        int estado_esq, estado_dir;
        MPI_Send(&estado, 1, MPI_INT, esquerda(rank), 0, MPI_COMM_WORLD);
        MPI_Send(&estado, 1, MPI_INT, direita(rank), 0, MPI_COMM_WORLD);

        MPI_Recv(&estado_esq, 1, MPI_INT, esquerda(rank), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&estado_dir, 1, MPI_INT, direita(rank), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (estado_esq != EATING && estado_dir != EATING) {
            estado = EATING;
            printf("Filósofo %d comendo.\n", rank);
            sleep(rand() % 2);
        } else {
            printf("Filósofo %d esperando.\n", rank);
        }

        estado = THINKING;
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
```

## Identificação do Bug
O uso de MPI_Send + MPI_Recv síncronos para todos os filósofos cria uma situação de espera circular, onde cada processo:
* Envia seu estado ao vizinho.
* Aguarda resposta do vizinho.
* Se todos enviarem simultaneamente, nenhum consegue chegar ao MPI_Recv.

Isso resulta em deadlock, todos ficam bloqueados indefinidamente.

### Situação de deadlock observada
Após mais de 20 execuções do código, foi detectada uma sequência de 5 filósofos consecutivos no estado com fome, sem progresso posterior evidência de deadlock:
```
“Sequência de 5 com fome encontrada na linha 6 até 10”
```
## 🛠️ Correção Implementada
Para resolver o problema, utilizamos comunicação não bloqueante:
* Substituímos MPI_Send por MPI_Isend (envio assíncrono).
* Usamos MPI_Irecv com MPI_Waitall para aguardar a chegada de todas as mensagens antes de decidir se o filósofo pode comer.


Isso eliminou a espera circular e garantiu que nenhum filósofo ficasse bloqueado indefinidamente.
Benefícios da correção
 
 * Elimina a espera circular (deadlock).
 * Permite maior paralelismo, já que envios não bloqueiam o processo.
 * Mantém a lógica de verificação dos vizinhos.
 * Garante sincronização ao final de cada rodada com MPI_Barrier.

## ⏱️ Métricas de Sincronização
Com a versão corrigida, medimos o tempo médio de espera de cada filósofo antes de conseguir comer:
```
Filósofo 0: tempo médio de espera = 0.300041 segundos em 10 refeições.
Filósofo 1: tempo médio de espera = 0.300031 segundos em 10 refeições.
Filósofo 2: tempo médio de espera = 0.500024 segundos em 10 refeições.
Filósofo 3: tempo médio de espera = 0.200032 segundos em 10 refeições.
Filósofo 4: tempo médio de espera = 0.600043 segundos em 10 refeições.
```
### 📊 Análise:
Houve variação natural no tempo de espera, com o filósofo 4 apresentando maior contenção (0.6s).
Nenhum deadlock foi observado após a correção, confirmando que o bug foi resolvido.
O balanceamento não é perfeito (o filósofo 3 comeu mais rápido em média), mas o sistema se manteve responsivo.

## ⚙️ Automação da Execução
Foi criado o script auto.sh para executar 10 rodadas consecutivas do programa, salvando as saídas em arquivos com timestamp:
```
#!/bin/bash

for i in {1..10}
do
    timestamp=$(date +"%Y-%m-%d_%H-%M-%S")
    saida="saida_${timestamp}.txt"
    echo "Executando rodada $i - Saída -> $saida"
    mpirun --oversubscribe -np 5 ./filosofos > "$saida"
    sleep 1
done
```
#### Comandos executados:
```
chmod +x auto.sh
./auto.sh
```
#### Saída típica:
```
Executando rodada 1 - Saída -> saida_2025-09-18_20-45-15.txt
Executando rodada 2 - Saída -> saida_2025-09-18_20-45-34.txt
...
Executando rodada 10 - Saída -> saida_2025-09-18_20-48-18.txt
```

### 📊 Análise das Saídas
Para detectar deadlocks, foi criado o seguinte comando awk:
```
awk '/com fome/ {c++} !/com fome/ {c=0} c==5 {print "Sequência de 5 com fome encontrada na linha", NR-4,"até", NR; c=0}' saida.txt
```
Depois, foi feito um loop para aplicar o awk em cada saida_*.txt, gerando arquivos de análise analise_saida_*.txt.
Para verificar quais arquivos apresentavam deadlock:
```
wc -l analise_saida_*.txt
```
#### Saída obtida:
```
0 analise_saida_2025-09-18_20-42-15.txt
1 analise_saida_2025-09-18_20-42-37.txt
...
1 total
```
Evidência do deadlock:
```
cat analise_saida_2025-09-18_20-42-37.txt
```
Resultado:
```
Sequência de 5 com fome encontrada na linha 6 até 10
```

## ⏱️ Observações de Sincronização
Houve várias execuções sem deadlock (arquivos de análise com 0 linhas).
Em uma execução, foi identificada uma sequência completa de filósofos com fome.
Após a correção no código com MPI_Isend e MPI_Irecv, não foram detectadas novas ocorrências de deadlock nas execuções posteriores.


## ✅ Conclusão
Bug identificado: Espera circular causada por uso de MPI_Send bloqueante em todos os processos simultaneamente.

Solução aplicada: Uso de comunicação não bloqueante (MPI_Isend e MPI_Irecv), eliminando deadlock.

Validação: Foi necessário rodar mais de 20 execuções para capturar o comportamento problemático. Após a correção, o sistema se manteve estável.

Métricas: Tempos médios de espera confirmaram sincronização funcional, com pequenas diferenças de contenção entre filósofos.

Ferramentas usadas: awk para detecção de padrões, wc -l para contagem de ocorrências e scripts em Bash para automação.


## 📂 Evidências entregues:
* Arquivos saida_*.txt com logs das execuções.
* Arquivos analise_saida_*.txt mostrando onde houve deadlock.
* Capturas de tela (ou saída de terminal) mostrando filósofos pensando, com fome, comendo e esperando.
* Tempos médios de espera de cada filósofo, comprovando ausência de deadlock após correção.
* Código original e código corrigido.


