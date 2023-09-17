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
	int inicioM;
	int inicioN;
};

void* multMatrix(void* arg);

int main(int argc, char **argv)
{
    int nMatrix = argc - 2;
    // checando o núme de arquivos passados
    if (nMatrix != 3)
    {
        printf("Erro: o número de parâmetros passados diferente de 3!");
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
        printf("linhas: %d\ncolunas: %d\n", lin[i], col[i]);
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
    printf("p = %d\n", p);
    int nThreads = lin[0] * col[1] / p;
    printf("nThreads = %d\n", nThreads);
    pthread_t threads[nThreads];
    int inicioLin = 0;
    int inicioCol = 0;
    struct argMultMatrix arg = {matrizes, lin, col, p, inicioLin, inicioCol};
    printf("criou a struct!\n");
    struct argMultMatrix* argumentos = &arg;

    for (int i = 0; i < nThreads; i++)
    {
        printf("iterando no for, i = %d\n", i);
        if (inicioCol >= col[1] && inicioLin < lin[0]) //se a linha estourar, vamos pra próxima linha e para na penúltima linha
        {
            inicioLin++;
            inicioCol = inicioCol % col[1]; //a linha não pode ser maior do que a linha
        } 
        pthread_create(&threads[i], NULL, multMatrix, argumentos);
        inicioCol += p;

        // se o número de elementos não for divisível por p e for a última linha, vai ser só o resto
        if (inicioLin == lin[0] - 1 && (lin[0] * col[1]) % p != 0)
        {
            argumentos->p = (lin[0] * col[1]) % p;
            pthread_create(&threads[p], NULL, multMatrix, argumentos);
        }
    }

    free(matrizes[0]);
    free(matrizes[1]);
    return 0;
}


void* multMatrix(void* arg)
{
    printf("abriu a thread!\n");
    struct argMultMatrix* arguments = (struct argMultMatrix *) arg;
    int linha = arguments->inicioM;
    
    // Criando a resultante com malloc
    int** resultante;
    resultante = (int**)calloc(arguments->lin[0], sizeof(int**));
    for (int j = 0; j < arguments->lin[0]; j++)
    {
        resultante[j] = (int*)calloc(arguments->col[1], sizeof(int*));
    }
    // achando o ponto certo
    for (int i = arguments->inicioN; i < arguments->p; i++)
    {
        if (i >= arguments->col[1])
        {
            linha++;
        } 
        resultante[linha][i % arguments->lin[0]] = 0;

        // itera no que é comum às duas matrizes, já se sabe a coluna e a linha
        for (int j = 0; j < arguments->col[0]; j++) 
        {
            resultante[linha][i % arguments->lin[0]] += arguments->matrizes[0][linha][j] * arguments->matrizes[1][j][i % arguments->lin[0]]; 
        }
        printf("%d ", resultante[linha][i % arguments->lin[0]]);
    }
    printf("\n");
    pthread_exit(NULL);
}
