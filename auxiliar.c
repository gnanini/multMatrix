#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
    FILE *fp1;
    FILE *fp2;
    fp1 = fopen("saveM1", "w");
    fp2 = fopen("saveM2", "w");

    //tranformar os argumentos em int
    int listaDimensoes[argc];
    for (int i = 1; i < argc; i++)
    {
        listaDimensoes[i] = atoi(argv[i]);
    }
    //error check
    if (listaDimensoes[2] != listaDimensoes[3])
    {
        printf("Erro: impossível multiplicar essas matrizes, número de colunas de m1, diferente do número de linhas de m2\n");
        exit(1);
    }

    //criando as matrizes
    int m1[listaDimensoes[1]][listaDimensoes[2]]; 
    int m2[listaDimensoes[3]][listaDimensoes[4]];
    
    //populando as matrizes podia fazer uma função, mas assim tá mais simples
    time_t t;
    srand((unsigned) time(&t));

    fprintf(fp1, "%d %d", listaDimensoes[1], listaDimensoes[2]);
    fprintf(fp1, "\n");

    for (int i = 0; i < listaDimensoes[1]; i++)
    {
        for (int j = 0; j < listaDimensoes[2]; j++)
        {
            m1[i][j] = rand() % 100;
            fprintf(fp1, "%d ", m1[i][j]);
        }
        fprintf(fp1, "\n");
    }
    fclose(fp1);


    fprintf(fp2, "%d %d", listaDimensoes[3], listaDimensoes[4]);
    fprintf(fp2, "\n");

    for (int i = 0; i < listaDimensoes[3]; i++)
    {
        for (int j = 0; j < listaDimensoes[4]; j++)
        {
            m2[i][j] = rand() % 100;
            fprintf(fp2, "%d ", m2[i][j]);
        }
        fprintf(fp2, "\n");
    }
    fclose(fp2);
    return 0;
}
