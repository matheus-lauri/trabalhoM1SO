#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <windows.h>

static pthread_mutex_t usando_contagem = PTHREAD_MUTEX_INITIALIZER;
static volatile int sair = 0; // Variável indicando se o usuário deseja sair

static int cont_itens = 0;
static int cont_peso = 0;

void *esteira1(void)
{
    while (!sair)
    {
        pthread_mutex_lock(&usando_contagem);
        cont_itens++;
        cont_peso += 5;
        pthread_mutex_unlock(&usando_contagem);
        Sleep(2000);
    }
}

void *esteira2(void)
{
    while (!sair)
    {
        pthread_mutex_lock(&usando_contagem);
        cont_itens++;
        cont_peso += 2;
        pthread_mutex_unlock(&usando_contagem);
        Sleep(1000);
    }
}

void *display(void)
{
    int aux = 0;
    int atualizacao = 0;
    while (!sair)
    {
        system("cls");
        printf("--MONITORAMENTO DA ESTEIRA--\n\n");
        pthread_mutex_lock(&usando_contagem);
        printf("Quantidade total de itens: %d\n", cont_itens);
        if (cont_itens >= atualizacao + 500)
        {
            aux += cont_peso;
            atualizacao += 500;
        }
        printf("Peso total: %d\n", aux);
        pthread_mutex_unlock(&usando_contagem);
        Sleep(2000);
    }
}

int main(void)
{
    DWORD start_time, end_time;

    // Cria as threads
    pthread_t thread_esteira1, thread_esteira2, thread_display;
    pthread_create(&thread_esteira1, NULL, (void *)esteira1, NULL);
    pthread_create(&thread_esteira2, NULL, (void *)esteira2, NULL);
    pthread_create(&thread_display, NULL, (void *)display, NULL);

    //Inicia o contador de tempo
    start_time = GetTickCount();

    // Aguarda o usuario pressionar 'q' para sair
    char ch;
    while (1)
    {
        ch = getchar();
        if (ch == 'q' || ch == 'Q')
        {
            sair = 1;
            break;
        }
    }

    // Para as threads
    pthread_cancel(thread_esteira1);
    pthread_cancel(thread_esteira2);
    pthread_cancel(thread_display);

    // Espera as threads terminarem
    pthread_join(thread_esteira1, NULL);
    pthread_join(thread_esteira2, NULL);
    pthread_join(thread_display, NULL);

    end_time = GetTickCount();

    // Calcula o tempo total de execução e imprime
    double elapsed_time = ((double)(end_time - start_time) / 1000.) - 2; // convertendo para segundos
    printf("Tempo total da execucao: %f segundos\n", elapsed_time);

    return 0;
}