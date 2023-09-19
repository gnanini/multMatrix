#/bin/bash
P=(1280000 960000 640000 320000 2560000 3840000 5120000)
T=(threads processos)

for t in ${T[@]};
do
    for k in ${P[@]};
    do
        for i in {0..9};
        do
            rm output/"$t/"*
            "./$t" saveM1 saveM2  "$k"
            grep s output/"$t"/* >> testes/"$t"/"$i-3200x3200-$k"
        done
    done
done
#for i in {0..9}; do ./sequencial saveM1 saveM2 && grep s output/sequencial >> testes/sequencial2400x2400 && rm output/sequencial; done
