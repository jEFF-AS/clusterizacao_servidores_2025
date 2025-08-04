#include <stdio.h>

int main() {
    int numeros[5]; // vetor com 5 posições
    int i, j, temp;

    // Pede os números ao usuário
    printf("Digite 5 números:\n");
    for(i = 0; i < 5; i++) {
        printf("Número %d: ", i + 1);
        scanf("%d", &numeros[i]);
    }

    // Ordenando com Bubble Sort
    for(i = 0; i < 5; i++) {
        for(j = 0; j < 4; j++) {
            if(numeros[j] > numeros[j + 1]) {
                // Troca os dois números de lugar
                temp = numeros[j];
                numeros[j] = numeros[j + 1];
                numeros[j + 1] = temp;
            }
        }
    }

    // Mostra os números ordenados
    printf("\nNúmeros em ordem crescente:\n");
    for(i = 0; i < 5; i++) {
        printf("%d ", numeros[i]);
    }

    printf("\n");

    return 0;
}
