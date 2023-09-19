#/bin/bash

for i in {1..9};
do
    ./sequencial saveM1 saveM2;
    grep s output/sequencial >> testes/sequencial/"3200x3200_$i"
done
#for i in {0..9}; do ./sequencial saveM1 saveM2 && grep s output/sequencial >> testes/sequencial2400x2400 && rm output/sequencial; done

