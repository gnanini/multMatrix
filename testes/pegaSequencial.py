import os
ps = ["3200"]

for p in ps:
    print(p)
    tempos = ""
    soma = 0
    k = 0
    for file in os.listdir("sequencial"):
        if p in file:
            #print(file)
            f = open("sequencial" +"/" +file, "r")
            #print(f.read()[:-2])
            soma += float(f.read()[:-2])
            k += 1
            f.close()
    print(soma / k)
    f = open(f"medias/mediaTempo-sequencial-{p}", "w")
    #print(len(tempos))
    media = soma / k
    f.write(str(media))
    f.close()
