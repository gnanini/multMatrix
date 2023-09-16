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
    }
    int** matrizes[argc-1]; // é criando um array de matrizes mesmo
    int lin;
    int col;
    for (int i = 0; i < argc-1; i++)
    {
        lin = atoi(strtok(matrizTexto[i], " \n"));
        col = atoi(strtok(NULL, " \n"));
        printf("linhas: %d\ncolunas: %d\n", lin, col);
        matrizes[i] = (int**)calloc(lin, sizeof(int));

        for (j = 0; j < lin; j++)
        {
            matrizes[i][j] = (int*)calloc(col, sizeof(int));
            for (int k = 0; k < col; k++) // criamos e já populamos a matriz
            {
                matrizes[i][j][k] = atoi(strtok(NULL, " \n"));
            }
        }
    }
    for (int i = 0; i < argc-1;i++)
    {
        for (j = 0; matrizes[i][j] != NULL; j++)
        {
            for (int k = 0; matrizes[i][j][k] != '\0'; k++)
            {
                printf ("%d ", matrizes[i][j][k]);
            }
            printf("\n");
        }
        free(matrizTexto[i]);
        printf("\n");
    }
    /*
    //tranformar os argumentos em int
    int listaDimensoes[argc];
    for (int i = 1; i < argc; i++)
    {
        listaDimensoes[i] = atoi(argv[i]);
    }
    //error check
    if (listaDimensoes[2] != listaDimensoes[3])
    {
        printf("impossível multiplicar essas matrizes, número de colunas de m1, diferente do número de linhas de m2\n");
        return 1;
    }

    //criando as matrizes
    int m1[listaDimensoes[1]][listaDimensoes[2]]; 
    int m2[listaDimensoes[3]][listaDimensoes[4]];
    int resultante[listaDimensoes[1]][listaDimensoes[4]];
    
    //populando as matrizes podia fazer uma função, mas assim tá mais simples
    time_t t;
    srand((unsigned) time(&t));

    for (int i = 0; i < listaDimensoes[1]; i++)
    {
        for (int j = 0; j < listaDimensoes[2]; j++)
        {
            m1[i][j] = rand() % 100;
            printf("%d ", m1[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    for (int i = 0; i < listaDimensoes[3]; i++)
    {
        for (int j = 0; j < listaDimensoes[4]; j++)
        {
            m2[i][j] = rand() % 100;
            printf("%d ", m2[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    //começar a multiplicação
    for (int m = 0; m < listaDimensoes[1]; m++)
    {
        for (int n = 0; n < listaDimensoes[4]; n++)
        {
            resultante[m][n] = 0;
            for (int k = 0; k < listaDimensoes[3]; k++) // podia ser listaDimensoes[2]
            {
                resultante[m][n] += m1[m][k] * m2[k][n]; 
            }
            printf("%d\t", resultante[m][n]);
        }
        printf("\n");
    }
    */
    return 0;
}
