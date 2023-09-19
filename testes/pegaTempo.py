import os


dirs = ["processos", "threads"]
tempos = []

for directory in dirs:
    for file in os.listdir(directory):
        if "2560000" in file:
            f = open(directory +"/" +file, "r")
            tempos.append(f.readlines())
            f.close()
filtrados = []
temp = ""
for item in tempos:
    for linha in item:
        for char in linha:
            if char in "01234567890.":
                temp = temp + char
            if char == ':':
                temp = ""
            if char == '\n':
                filtrados.append(temp)
                temp = ""
soma = 0
for item in filtrados:
    soma += float(item)
#
print(f"media = {soma / len(filtrados)}s")
