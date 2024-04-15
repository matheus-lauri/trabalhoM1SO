// Processo Gerenciador
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#define BUFFER_SIZE 1024
#define PIPE_NAME_1 "\\\\.\\pipe\\esteira1"
#define PIPE_NAME_2 "\\\\.\\pipe\\esteira2"

void atualiza_display(int qtd, int kg)
{
    static int somatorio = 0;
    static int peso_antigo = 0;
    int aux = peso_antigo;
    int intervalo = 500;

    printf("\033[H\033[J");
    printf("--MONITORAMENTO DAS ESTEIRAS--\n\n");
    printf("Quantidade total de itens: %d\n", qtd);

    if (qtd >= somatorio + intervalo)
    {
        peso_antigo = kg;
        aux = kg;
        somatorio += intervalo;
    }

    printf("Peso total: %d\n", aux);
}

int main()
{
    HANDLE hPipe1, hPipe2;

    // Variáveis de contagem da esteira
    int qtd_anterior, kg_anterior, qtd_total, kg_total, qtd1 = 0, qtd2 = 0, kg1 = 0, kg2 = 0;

    // Controle do input do usuário para fechar a execução do código
    char ch;

    DWORD start_time, end_time, bytesAvailable1, bytesAvailable2;

    // Processo de criação e conexão dos pipes
    hPipe1 = CreateNamedPipe(PIPE_NAME_1,
                             PIPE_ACCESS_DUPLEX,
                             PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                             1,
                             BUFFER_SIZE,
                             BUFFER_SIZE,
                             0,
                             NULL);

    if (hPipe1 == INVALID_HANDLE_VALUE)
    {
        printf("Falha em criar o pipe 1. Codigo do erro: %d\n", GetLastError());
        return 1;
    }

    hPipe2 = CreateNamedPipe(PIPE_NAME_2,
                             PIPE_ACCESS_DUPLEX,
                             PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                             1,
                             BUFFER_SIZE,
                             BUFFER_SIZE,
                             0,
                             NULL);

    if (hPipe2 == INVALID_HANDLE_VALUE)
    {
        printf("Falha em criar o pipe 2. Codigo do erro: %d\n", GetLastError());
        return 1;
    }

    printf("\n\nAguardando conexão da esteira 1...\n");

    if (!ConnectNamedPipe(hPipe1, NULL))
    {
        printf("Falha em conectar ao pipe da esteira 1. Codigo do erro: %d\n", GetLastError());
        CloseHandle(hPipe1);
        return 1;
    }

    printf("\n\nAguardando conexao da esteira 2...\n");

    if (!ConnectNamedPipe(hPipe2, NULL))
    {
        printf("Falha em conectar ao pipe da esteira 2. Codigo do erro: %d\n", GetLastError());
        CloseHandle(hPipe2);
        return 1;
    }

    // Inicia o contador de tempo
    start_time = GetTickCount();

    while (1)
    {
        // Loop para capturar valores do pipe 1
        while (PeekNamedPipe(hPipe1, NULL, 0, NULL, &bytesAvailable1, NULL) && bytesAvailable1 > 0) //Verifica se há dados disponíveis no pipe
        {
            ReadFile(hPipe1, &qtd1, sizeof(qtd1), NULL, NULL);
            ReadFile(hPipe1, &kg1, sizeof(kg1), NULL, NULL);
        }

        // Loop para capturar valores do pipe 2
        while (PeekNamedPipe(hPipe2, NULL, 0, NULL, &bytesAvailable2, NULL) && bytesAvailable2 > 0) //Verifica se há dados disponíveis no pipe
        {
            ReadFile(hPipe2, &qtd2, sizeof(qtd2), NULL, NULL);
            ReadFile(hPipe2, &kg2, sizeof(kg2), NULL, NULL);
        }

        qtd_total = qtd1 + qtd2;
        kg_total = kg1 + kg2;

        atualiza_display(qtd_total, kg_total);

        // Encerramento do código
        if (_kbhit())
        {
            ch = getchar();
            if (ch == 'q' || ch == 'Q')
            {
                CloseHandle(hPipe1);
                CloseHandle(hPipe2);

                // Finaliza o contador de tempo
                end_time = GetTickCount();

                // Calcula o tempo total de execução e imprime
                double elapsed_time = ((double)(end_time - start_time) / 1000.0) - 2; // convertendo para segundos
                printf("Tempo total da execucao: %f segundos\n", elapsed_time);

                return 0;
            }
        }

        Sleep(2000); // Espera 2 segundos
    }

    return 0;
}
