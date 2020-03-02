import math


def prob1():
    a = int(input("a="))
    b = int(input("b="))
    c = int(input("c="))

    d = (b ** 2) - (4 * a * c)
    if d < 0:
        print("fara radacini")
    elif d == 0:
        x = (-b) / (2 * a)
        print("unica solutie: ", x)
    else:
        x1 = (-b + d ** 0.5) / (2 * a)
        x2 = (-b - d ** 0.5) / (2 * a)
        print("solutiile: ", x1, " ", x2)


def cmmdc(x, y):
    while x * y != 0:
        if x > y:
            x -= y
        else:
            y -= x

    return x


def prob2():
    L1 = int(input("L1="))
    L2 = int(input("L2="))
    # d = math.gcd(L1, L2)
    d = cmmdc(L2, L1)
    print("lutura placi: ", d)
    print("nr placi: ", (L1 // d) * (L2 // d))


def prob3():
    '''
    x = int(input("x, lunginme init = "))
    n = int(input("n, sarituri = "))
    p = int(input("p, procent = "))
    m = int(input("m, nr sarituri = "))
    '''
    x = 20
    n = 10
    p = 10
    m = 20

    dist = 0
    # x = x * ((1 - p/100)**n2)

    for k in range(0, m // n):
        dist += (1 - (p / 100)) ** k
    dist *= x
    dist *= 10
    print("dist: ", dist)


def prob4Liste():
    n = 6
    valori = [4.25, 4.05, 4.25, 4.48, 4.30, 4.30]

    """
    n = int(input("n="))
    valori = []
    for i in range(0,n):
        val = int(input("val="))
        valori.insert(i, val)
    """

    diffs = []
    for i in range(0, n - 1):
        dif = abs(valori[i] - valori[i + 1])
        diffs.insert(i, dif)

    maxDif = 0
    zi = 0
    for i in range(0, n - 1):
        if maxDif < diffs[i]:
            maxDif = diffs[i]
            zi = i + 1

    print("zile: ", zi, zi+1)
    print("valoare: ", round(maxDif, 2))
    #print("valoare: %.2f ".format())


def prob4():
    prevCurs = 0
    curs = 0
    zi = 0
    maxdif = 0

    # hard coded input
    inputVec = [4.25, 4.05, 4.25, 4.48, 4.30, 4.30]
    #n = int(input("n="))
    n = len(inputVec)
    for i in range(0, n):
        #curs = int(input("curs="))
        curs = inputVec[i]
        if prevCurs != 0:
            if maxdif < abs(curs - prevCurs):
                maxdif = abs(curs - prevCurs)
                zi = i
        prevCurs = curs

    print("zile: ", zi, zi+1)
    print("valoare: ", round(maxdif, 2))


def prob5():
    #inputVec = [4, 7, 1, 32, 8, 5, 10, 4]
    inputVec = [9, 1, 3, 4, 6]
    #n = int(input("n="))
    n = len(inputVec)
    max1 = 0
    max2 = 0
    for i in range(0, n):
        #val = int(input("val="))
        val = inputVec[i]
        if max1 < val:
            max1 = val
        elif max2 < val:
            max2 = val

    print(max1, max2)

def ceaMaiMareCifra(n):
    cmax = 0
    while n != 0:
        cif = n % 10
        if cmax < cif:
            cmax = cif
        n /= 10
    return cmax

def func(beg, end, step, n):
    nr = 0
    for cif in range(beg, end, step):
        m = n
        deCateOri = 0
        while m != 0:
            if m % 10 == cif:
                deCateOri += 1
            m //= 10
        for _ in range(0, deCateOri):
            nr *= 10
            nr += cif
    return nr

def prob6():
    #maxNr = 0
    #minNr = 0
    #n = int(input("n="))
    #cifra = 0
    #while n != 0:
        #cif = ceaMaiMareCifra(n)
        #maxNr *= 10
        #maxNr += cif
    n = 812383
    maxNr = func(0, 10, 1, n) 
    minNr = func(9, -1, -1, n)
    
    print(maxNr, minNr)
