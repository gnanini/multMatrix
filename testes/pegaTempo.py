import os


dirs = ["processos", "threads"]
ps=["1280000", "960000", "640000", "320000", "2560000", "3840000", "5120000"]

for directory in dirs:
    print(directory)
    for p in ps:
        tempos = ""
        for file in os.listdir(directory):
            if p in file:
                print(file)
                f = open(directory +"/" +file, "r")
                #tempos.append(f.readlines())
                for line in f.readlines():
                    #print(line)
                    tempos += line
                f.close()
        #print(tempos.split("\n"))
        #print(tempos)
        tempos = tempos.split("s\n")[:-1]
        soma = 0
        for item in tempos:
            #print(item)
            temp = ""
            for char in item:
                temp += char
                if char == ':':
                    temp = ""
            soma += float(temp)
        f = open(f"medias/mediaTempo-{directory}-{p}", "w")
        #print(len(tempos))
        media = soma / len(tempos)
        f.write(str(media))
        f.close()
