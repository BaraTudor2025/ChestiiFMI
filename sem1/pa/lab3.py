import io


def prob2():
    in_file = open("test.in", 'r')
    out_file = open("test.out", 'w')
    ecuations = in_file.readlines()
    nota = 1
    for s_ecu in ecuations:
        ec = s_ecu.replace('\n', '')
        s1 = ec.split("*")
        n1 = int(s1[0])

        s2 = s1[1].split("=")
        n2 = int(s2[0])
        n3 = int(s2[1])

        if n1 * n2 == n3:
            nota += 1
            out_file.write(ec + " Corect\n")
        else:
            out_file.write('{} Gresti {}\n'.format(ec, n1 * n2))
    out_file.write("Nota " + str(nota))


def prob3():
    text = "Azi am cumpărat 5.5 kg de mere cu 2.2 RON kilogramul și 3 pâini a câte 5 RON fiecare."
    text = text.split(' ')
    numere = []
    for cuv in text:

        #float
        i = cuv.find('.')
        if i != -1:
            if cuv[:i].isnumeric() and cuv[i+1:].isnumeric():
                numere.append(float(cuv))

        #int
        if cuv.isnumeric():
            numere.append(int(cuv))

    suma = 0
    for i in range(0,  len(numere), 2):
        suma += numere[i] * numere[i+1]

    print(suma)


def prob4():
    text = "7 9 90 43 3 65 23 45 89"
    nums = [int(x) for x in text.split(' ')]
    maxNum = max(nums)
    nums.remove(maxNum)
    print(maxNum, max(nums))


    
