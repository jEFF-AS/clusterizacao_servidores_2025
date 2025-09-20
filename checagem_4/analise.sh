#!/bin/bash

for arquivo in saida_*.txt
do
    resultado="analise_${arquivo}"
    echo "Analisando $arquivo -> resultado em $resultado"
    awk '/com fome/ {c++} !/com fome/ {c=0} c==5 {print "Sequência de 5 com fome encontrada na linha", NR-4,"até", NR; c=0}' "$arquivo" > "$resultado"
done
