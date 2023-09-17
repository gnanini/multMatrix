#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
    // checando o núme de arquivos passados
    if (argc != 3)
    {
        printf("Erro: o número de arquivos passados como parâmetro deve ser exatamente 2!");
    }
    //ler o arquivo
    FILE* fptr[argc-1];
    //nomes dos arquivos passados como argumento
    for (int i = 0; i < argc-1; i++)
    {
        fptr[i] = fopen(argv[i+1], "r");

        //error check
        if (fptr[i] == NULL)
        {
            printf("Erro: arquivo %d inválido!", i + 1);
            exit(0);
        }
    }
    // Lendo os arquivos
    char temp;
    char* matrizTexto[argc-1];
    int bufferSize = 100;
    int bufferIncremento;
    
    for (int i = 0; i < argc-1; i++)
    {
        matrizTexto[i] = (char*)calloc(bufferSize, sizeof(char));
    }
    int j;

    for (int i = 0; i < argc-1; i++)
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
    int** matrizes[argc-1]; // é criando um array de matrizes mesmo
    int lin[argc-1]; // podia ter feito um struct
    int col[argc-1]; // salvar o tamalho das linhas e colunas como array
    for (int i = 0; i < argc-1; i++)
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
    for (int i = 0; i < argc-1;i++)
    {
        for (j = 0; j < lin[i]; j++)
        {
            for (int k = 0; k < col[i]; k++)
            {
                printf ("%d ", matrizes[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
        free(matrizTexto[i]);
    }
    //error check
    if (col[0] != lin[1])
    {
        printf("impossível multiplicar essas matrizes, número de colunas de m1, diferente do número de linhas de m2\n");
        return 1;
    }
    //criando a resultante
    int resultante[lin[0]][col[1]];
    
    //começar a multiplicação
    printf("\n");
    for (int m = 0; m < lin[0]; m++)
    {
        for (int n = 0; n < col[1]; n++)
        {
            resultante[m][n] = 0;
            for (int k = 0; k < lin[1]; k++) // podia ser listaDimensoes[2]
            {
                resultante[m][n] += matrizes[0][m][k] * matrizes[1][k][n]; 
            }
            printf("%d\t", resultante[m][n]);
        }
        printf("\n");
    }
    //free(matrizes[0]);
    //free(matrizes[1]);
    return 0;
}
