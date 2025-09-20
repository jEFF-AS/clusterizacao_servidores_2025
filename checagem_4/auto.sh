#!/bin/bash

for i in {1..10}
do
    # Gera timestamp no formato: YYYY-MM-DD_HH-MM-SS
    timestamp=$(date +"%Y-%m-%d_%H-%M-%S")

    # Nome do arquivo de saída
    saida="saida_${timestamp}.txt"

    echo "Executando rodada $i - Saída -> $saida"

    # Executa o programa e redireciona a saída
    mpirun -np 5 --oversubscribe ./filosofos > "$saida"

    # Espera 1 segundo para evitar timestamps duplicados
    sleep 1
done
