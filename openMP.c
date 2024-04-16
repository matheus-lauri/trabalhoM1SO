// Implementacao com openMP (Codigo extra do trabalho)
#include <stdio.h>
#include <omp.h>
#include <unistd.h>
#include <windows.h>
#include <conio.h>

static int sair = 0; // Variável indicando se o usuário deseja sair

static int cont_itens = 0;
static int cont_peso = 0;

void esteira1(void)
{
    while (!sair)
    {
        #pragma omp critical
        {
            cont_itens++;
            cont_peso += 5;
        }
        sleep(2);
    }
}

void esteira2(void)
{
    while (!sair)
    {
        #pragma omp critical
        {
            cont_itens++;
            cont_peso += 2;
        }
        sleep(1);
    }
}

void display(void)
{
    int aux = 0;
    int atualizacao = 0;
    while (!sair)
    {
        system("cls");
        printf("--MONITORAMENTO DA ESTEIRA--\n\n");
        #pragma omp critical
        {
            printf("Quantidade total de itens: %d\n", cont_itens);
            if (cont_itens >= atualizacao + 500)
            {
                aux += cont_peso;
                atualizacao += 500;
            }
            printf("Peso total: %d\n", aux);
        }
        sleep(2);
    }
}

int main(void)
{
    DWORD start_time, end_time;

// Cria as threads
#pragma omp parallel sections
    {
        #pragma omp section
        {
            esteira1();
        }

        #pragma omp section
        {
            esteira2();
        }

        #pragma omp section
        {
            display();
        }
    }

    // Inicia o contador de tempo
    start_time = GetTickCount();

    // Aguarda o usuário pressionar 'q' para sair
    while (!sair)
    {
        if (_kbhit()) {
            char ch = getchar();
            if (ch == 'q' || ch == 'Q') {
                #pragma omp critical
                {
                    sair = 1;
                }
            }
        }
    }

    end_time = GetTickCount();

    // Calcula o tempo total de execução e imprime
    double elapsed_time = ((double)(end_time - start_time) / 1000.) - 2; // convertendo para segundos
    printf("Tempo total da execucao: %f segundos\n", elapsed_time);

    return 0;
}
