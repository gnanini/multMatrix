#/bin/bash
d=(100 200 400 800 1600 3200)
for i in ${d[@]};
do
    ./auxiliar "$i" "$i" "$i" "$i";
    ./sequencial saveM1 saveM2;
    grep s output/sequencial >> testes/sequencial/"iniciais$ix$i"
done
#for i in {0..9}; do ./sequencial saveM1 saveM2 && grep s output/sequencial >> testes/sequencial2400x2400 && rm output/sequencial; done

