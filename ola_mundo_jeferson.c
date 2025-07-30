#include <stdio.h>
#include <mpi.h>

/*
Relato de aula -- Jeferson

O primeiro passo da aula foi executar o WSL. Mas ouve um problema, o WSL nao executa. 
Instalei novamente o wsl via terminal, porem sem sucesso. Baixei o Ubuntu pelo Microsoft como alternativa e tambem nao tive sucesso.
Entao busquei rodar o wsl em outro computador, mas sem sucesso.
Voltei para o computador que eu estava antes, buscando solucionar o problema. 
No powershell usei o comando `wsl --install -d ubuntu`, e reiniciei o computador sendo esse o passo vital para o sucesso da execussao do WSL.

O WSL e permite executar um ambinte Linux diretamente no Windows, sem precisar instalar distribuicoes linux no disco fisico.
Instalamos o MPI , que permite que varios processos rode ao mesmo tempo em varios nucleos diferentes, CPUs e ate mesmo em computadores diferentes.
 Foi instalado o gcc, realizado um clone de um repositorio e executado o arquivo ola_mundo.

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
