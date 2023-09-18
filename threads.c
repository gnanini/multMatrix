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
    int p = atoi(argv[3]);
    int nThreads = lin[0] * col[1] / p;
    if (lin[0] * col[1] % p != 0)
    {
        nThreads++;
    }
    printf("nThreads = %d\n", nThreads);
    pthread_t threads[nThreads];
    struct argMultMatrix arg = {matrizes, lin, col, p, 0, 0};
    struct argMultMatrix* argumentos = &arg;

    for (int i = 0; i < nThreads; i++)
    {
        printf("thread %d\n", i);
        if (argumentos->inicioC >= col[1] && argumentos->inicioL < lin[0] - 1) //se a linha estourar, vamos pra próxima linha e para na penúltima linha
        {
            argumentos->inicioL++;
        } 
        // se o número de elementos não for divisível por p e for a última linha, vai ser só o resto
        //if (argumentos->inicioL == lin[0] - 1 && argumentos->inicioC + argumentos->p > col[1])
        //{
        //    printf("entrou na dita condição!\n");
        //    argumentos->p =  col[1] - (argumentos->inicioC + p);
        //}
        argumentos->inicioC = argumentos->inicioC % col[1]; //a linha não pode ser maior do que a linha
        printf("p = %d\n", argumentos->p);
            printf("inicioC = %d\n",argumentos->inicioC);
            printf("inicioL = %d\n",argumentos->inicioL);
        pthread_create(&threads[i], NULL, multMatrix, argumentos);
        pthread_join(threads[i], NULL);
        argumentos->inicioC += argumentos->p;
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
    printf("col = %d\n", arguments->col[1]);

    for (int i = 0; i < arguments->p; i++)
    {
        printf("i = %d ", i);
        if (arguments->lin[0] <= linha || arguments->col[1] <= coluna)
        {
            printf("saiu!!\n");
            pthread_exit(NULL);
        }
        if (coluna >= arguments->col[1])
        {
            linha++;
            coluna = coluna % arguments->col[1];
        } 
        soma = 0;
        // itera no que é comum às duas matrizes, já se sabe a coluna e a linha
        for (int j = 0; j < arguments->col[0]; j++) 
        {
            //resultante[linha][i + n % arguments->col[1]] += arguments->matrizes[0][linha][j] * arguments->matrizes[1][j][i + n % arguments->col[1]]; 
            soma += arguments->matrizes[0][linha][j] * arguments->matrizes[1][j][coluna]; 
        }
        //printf("%d ", resultante[linha][i + n % arguments->col[1]]);
        printf("%d\n", soma);
        coluna++;
    }
    printf("\n");
    printf("Cálculos concluídos!\n");
    pthread_exit(NULL);
}
