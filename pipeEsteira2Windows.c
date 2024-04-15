// Processo da contagem da Esteira 2
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <time.h>

#define PIPE_NAME "\\\\.\\pipe\\esteira2"

int main() {
    HANDLE hPipe;
    char ch;
    int qtd = 0, kg = 0;

    DWORD start_time, end_time;

    //Inicia o contador de tempo
    start_time = GetTickCount();

    // Conexão ao pipe
    hPipe = CreateFile(PIPE_NAME,
                       GENERIC_READ | GENERIC_WRITE,
                       0,
                       NULL,
                       OPEN_EXISTING,
                       0,
                       NULL);
    
    if (hPipe == INVALID_HANDLE_VALUE)
    {
        printf("Falha em abrir o pipe. Codigo do erro: %d\n", GetLastError());
        return 1;
    }

    int flags = _setmode(_fileno(stdin), _O_BINARY);
    if (flags == -1) {
        printf("Falha ao configurar o modo de entrada binário");
        return 1;
    }

    while (1) {
        qtd++;
        kg += 2;

        printf("qtd: %d ", qtd);
        printf("kg: %d\n", kg);

        // Envia quantidade e peso de forma consecutiva
        DWORD bytesWritten;
        if (!WriteFile(hPipe, &qtd, sizeof(qtd), &bytesWritten, NULL) || bytesWritten != sizeof(qtd)) {
            printf("Falha em escrever a quantidade no pipe. Codigo do erro: %d\n", GetLastError());
            CloseHandle(hPipe);
            return 1;
        }

        if (!WriteFile(hPipe, &kg, sizeof(kg), &bytesWritten, NULL) || bytesWritten != sizeof(kg)) {
            printf("Falha em escrever o peso no pipe. Codigo do erro: %d\n", GetLastError());
            CloseHandle(hPipe);
            return 1;
        }

        // Encerramento do código
        if (_kbhit()) {
            ch = getchar();
            if (ch == 'q' || ch == 'Q') {
                printf("\n\nPrograma finalizado");
                CloseHandle(hPipe);

                //Finaliza o contador de tempo
                end_time = GetTickCount();

                // Calcula o tempo total de execução e imprime
                double elapsed_time = ((double)(end_time - start_time) / 1000.0) - 2; // convertendo para segundos
                printf("Tempo total da execucao: %f segundos\n", elapsed_time);

                return 0;
            }
        }

        Sleep(1000); // Espera 1 segundo
    }

    return 0;
}
