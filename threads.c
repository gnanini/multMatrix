#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

struct argMultMatrix {
    int*** matrizes;
    int* lin;
	int* col;
	int p;
	int inicioL;
	int inicioC;
};

void* multMatrix(void* arg);

int main(int argc, char **argv)
{
    printf("argc = %d\n", argc);
    int nMatrix = argc - 2;
    // checando o núme de arquivos passados
    if (nMatrix != 2)
    {
        printf("Erro: o número de parâmetros passados diferente de 2!");
    }
    //ler o arquivo
    FILE* fptr[nMatrix];
    //nomes dos arquivos passados como argumento
    for (int i = 0; i < nMatrix; i++)
    {
        fptr[i] = fopen(argv[i+1], "r");

        //error check
        if (fptr[i] == NULL)
        {
            printf("Erro: arquivo %d inválido!", i + 1);
            exit(1);
        }
    }
    // Lendo os arquivos
    char temp;
    char* matrizTexto[nMatrix];
    int bufferSize = 100;
    int bufferIncremento;
    
    for (int i = 0; i < nMatrix; i++)
    {
        matrizTexto[i] = (char*)calloc(bufferSize, sizeof(char));
    }
    int j;

    for (int i = 0; i < nMatrix; i++)
    {
        j = 0;
        bufferIncremento = bufferSize;
        while(!feof(fptr[i]))
        {
            if (j % bufferIncremento == 0 && j > 0)
            {
                bufferIncremento += bufferSize;
                matrizTexto[i] = (char*) realloc(matrizTexto[i], bufferIncremento * sizeof(char));
            }
            temp = fgetc(fptr[i]);
            matrizTexto[i][j] = temp;
            j++;
        }
        matrizTexto[i][j] = '\0';
        fclose(fptr[i]);
    }
    int** matrizes[nMatrix]; // é criando um array de matrizes mesmo
    int lin[nMatrix]; // podia ter feito um struct
    int col[nMatrix]; // salvar o tamanho das linhas e colunas como array
    //parseando o texto para matrizes de ints
    for (int i = 0; i < nMatrix; i++)
    {
        lin[i] = atoi(strtok(matrizTexto[i], " \n"));
        col[i] = atoi(strtok(NULL, " \n"));
        matrizes[i] = (int**)calloc(lin[i], sizeof(int**));

        for (j = 0; j < lin[i]; j++)
        {
            matrizes[i][j] = (int*)calloc(col[i], sizeof(int*));
            for (int k = 0; k < col[i]; k++) // criamos e já populamos a matriz
            {
                matrizes[i][j][k] = atoi(strtok(NULL, " \n"));
            }
        }
    }
    for (int i = 0; i < nMatrix;i++)
    {
        free(matrizTexto[i]);
    }
    //error check
    if (col[0] != lin[1])
    {
        printf("impossível multiplicar essas matrizes, número de colunas de m1, diferente do número de linhas de m2\n");
         exit(1);
    }

    //Criando as threads
    time_t start, end;
    int p = atoi(argv[3]);
    printf("p = %d\n", p);
    int nThreads = lin[0] * col[1] / p;
    printf("nThreads = %d\n", nThreads);
    pthread_t threads[nThreads];
    struct argMultMatrix arg = {matrizes, lin, col, p, 0, 0};
    struct argMultMatrix* argumentos = &arg;

    for (int i = 0; i < nThreads; i++)
    {
        printf("thread %d\n", i);
        if (argumentos->inicioC >= col[1] && argumentos->inicioL < lin[1] - 1) //se a linha estourar, vamos pra próxima linha e para na penúltima linha
        {
            argumentos->inicioL++;
            argumentos->inicioC = argumentos->inicioC % col[1]; //a linha não pode ser maior do que a linha
        } 
        // se o número de elementos não for divisível por p e for a última linha, vai ser só o resto
        if (argumentos->inicioL == lin[0] - 1 && (lin[0] * col[1]) % p != 0)
        {
            printf("entrou na dita condição!\n");
            argumentos->inicioC = argumentos->inicioC % col[1]; //a linha não pode ser maior do que a linha
        }
        printf("p = %d\n", argumentos->p);
        pthread_create(&threads[i], NULL, multMatrix, argumentos);
        time(&start);
        pthread_join(threads[i], NULL);
        time(&end);
        int cputime = end - start;
        printf("Tempo de CPU usado pela thread: %d\n", cputime);
        
    }
    free(matrizes[0]);
    free(matrizes[1]);
    return 0;
}


void* multMatrix(void* arg)
{
    struct argMultMatrix* arguments = (struct argMultMatrix *) arg;
    int linha = arguments->inicioL;
    int coluna = arguments->inicioC;
    printf("coluna %d\n", coluna);
    int soma;
    printf("p = %d\n", arguments->p);
    for (int i = 0; i < arguments->p; i++)
    {
        if (i + coluna >= arguments->col[1])
        {
            linha++;
        } 
        soma = 0;
        // itera no que é comum às duas matrizes, já se sabe a coluna e a linha
        for (int j = 0; j < arguments->col[0]; j++) 
        {
            //resultante[linha][i + n % arguments->col[1]] += arguments->matrizes[0][linha][j] * arguments->matrizes[1][j][i + n % arguments->col[1]]; 
            soma += arguments->matrizes[0][linha][j] * arguments->matrizes[1][j][(i + coluna) % arguments->col[1]]; 
        }
        //printf("%d ", resultante[linha][i + n % arguments->col[1]]);
        printf("%d ", soma);
    }
    printf("\n");
    long long start_microseconds = start_time.tv_sec * 1000000 + start_time.tv_usec;
        long long end_microseconds = end_time.tv_sec * 1000000 + end_time.tv_usec;
        long long cpu_time_microseconds = (end_usage.ru_utime.tv_sec * 1000000 + end_usage.ru_utime.tv_usec) - (start_usage.ru_utime.tv_sec * 1000000 + start_usage.ru_utime.tv_usec);
        argumentos->inicioC += argumentos->p;
        printf("CPU time for the thread: %lld microseconds\n", cpu_time_microseconds); // printando o tempo de uso
    printf("Cálculos concluídos!\n");
    pthread_exit(NULL);
}
