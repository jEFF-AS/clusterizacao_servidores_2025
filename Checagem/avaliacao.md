# Avaliação de Checagem — MPI em Laboratório
## Aplicações de clusterização em servidores

**Tema:** Comunicação entre processos com MPI (Scatter, Gather, rank, size)  
**Aluno:** Jéferson Alves Santos 
**Data:** 05/08/2025  

---

###  Objetivo

Executar, analisar e modificar um programa MPI que distribui dados entre processos, realiza cálculos locais e coleta os resultados para ordenação.

---

###  Arquivo base

O código que você deve utilizar está nesse diretorio:

```
media_mpi.c
```

Compile com:

```bash
mpicc media_mpi.c -o media_mpi
```

Execute com:

```bash
mpirun -np 4 ./media_mpi
```

---

### Parte 1 — Execução básica

#### 1.1. Saída esperada

Execute o programa com 4 processos. Copie aqui a saída do terminal:

```
Processo 0 enviando dado: 42 para processo 1  
Processo 1 recebeu dado: 42 do processo 0 
```

---

###  Parte 2 — Análise de funcionamento

#### 2.1. O que faz `MPI_Scatter` neste código?

_Resposta:_ O MPI_Scatter não faz nada nesse codigo, pois ele não é usado.

---

#### 2.2. Qual o papel de `MPI_Gather`?

_Resposta:_ O papel principal do MPI_Gather é reunir dados de vários processos em um único processo raiz.

---

#### 2.3. Por que a ordenação das médias acontece apenas no processo 0?

_Resposta:_ A ordenação das médias ocorre somente no processo 0, pois, em programas MPI, esse processo costuma 
ser encarregado de coordenar as atividades, compilar os dados de todos os processos (utilizando MPI_Gather) e executar operações finais, 
como ordenação e apresentação dos resultados. Isso previne trabalho redundante, uma vez que os outros processos normalmente manipulam apenas 
partes dos dados, e ordenar localmente em cada um deles seria ineficaz e desnecessário.

---

###  Parte 3 — Modificação

#### 3.1. Modifique o código para que **cada processo envie também seu maior valor local**, além da média.

Use `MPI_Gather` para coletar ambos os dados no processo 0.

 - Faça um **commit com sua modificação** e anexe abaixo o arquivo completo.

---

### 3.2. Copie aqui a saída do seu programa modificado:

```
Resultados coletados no processo 0:
Processo 0: Média = 59.20, Maior valor = 78
Processo 1: Média = 60.40, Maior valor = 99
Processo 2: Média = 45.20, Maior valor = 70
Processo 3: Média = 75.00, Maior valor = 96

Os dados são gerados de forma aleatória (com rand()), portanto, cada execução produz valores diferentes e, geralmente, maiores, variando de 0 a 99. 
Isso resulta em médias e máximos mais altos e variados.
```

---

### Análise com utilitários Linux

#### 4.1. Use o comando `time` para medir o tempo de execução do programa com 2, 4 e 6 processos.

Anote abaixo:

| Processos | Tempo (real) |
|-----------|--------------|
| 2         | 0m0.346s     |
| 4         | 0m0.370s     |
| 6         | 0m0.403s     |

#### 4.2. Use `htop` ou `top` para observar o uso de CPU. O uso foi balanceado entre os processos?

_Resposta:_

--- Analise primiraria não houve resultado concreto, algumas vezes ao executar os processos era mostra, porem nas tentativas seguintes parou de exibir.

#### 4.3. Use `strace`, `taskset` ou `MPI_Wtime` para investigar comportamento adicional do programa. Comente algo que tenha achado interessante:

_Resposta:_

---

**Boa prática!** 
