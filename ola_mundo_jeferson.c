#include <stdio.h>
#include <mpi.h>

/*
Relato de Aula – Jeferson

Na aula de hoje, o primeiro passo foi tentar executar o WSL (Windows Subsystem for Linux). 
No entanto, enfrentei um problema: o WSL não iniciava corretamente.

Tentei reinstalá-lo pelo terminal, mas não obtive sucesso. Como alternativa, baixei o Ubuntu pela Microsoft Store, porém o problema persistiu. 
Em seguida, testei executar o WSL em outro computador, também sem êxito.

Retornei então ao computador inicial e continuei buscando uma solução. No PowerShell, utilizei o comando wsl --install -d Ubuntu e reiniciei a máquina. 
Essa reinicialização foi essencial para que o WSL funcionasse corretamente.

O WSL permite executar um ambiente Linux diretamente no Windows, sem a necessidade de instalar uma distribuição Linux em um disco físico. 
Após a configuração bem-sucedida, instalamos o MPI (Message Passing Interface), uma ferramenta que permite a execução de múltiplos processos em diferentes núcleos, 
CPUs e até mesmo em computadores distintos.

Além disso, foi instalado o compilador gcc (GCC (GNU Compiler Collection) é um compilador um programa que transforma código-fonte escrito por você (por exemplo, em C, C++ e outras linguagens) 
em um programa executável que o computador consegue rodar.), feito o clone de um repositório e executado o arquivo ola_mundo.
*/

int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv); // Inicializa MPI
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Número total de processos
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // ID do processo atual

    printf("Olá do processo %d de %d\n", rank, size);

    MPI_Finalize(); // Finaliza MPI
    return 0;
}
