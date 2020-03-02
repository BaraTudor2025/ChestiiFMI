def prob1():
    textGresit = "caractedf nu sunt gdfle"
    sirGresit = "df"
    sirCorect = "re"
    print(textGresit.replace(sirGresit, sirCorect))


def prob2():
    text = "mama are mere ana are mere"
    texts = text.split(" ")
    for i in range(0, len(texts)):
        text = texts[i]
        print(text[0].capitalize() + text[1:])


def prob3():
    #sir = input("sir=")
    #s = input("s=")
    #t = input("t=")
    sir = "are ana are piersici mare are"
    s = "are"
    t = "avea"
    k = len(s)

    pos = sir.find(s)
    while pos != -1:
        front_cond = (pos == 0 or sir[pos-1] == ' ')
        back_cond = (pos + k == len(sir) or sir[pos + k] == ' ')
        if front_cond and back_cond:
            sir = sir[:pos] + sir[pos:].replace(s, t, 1)
        pos = sir.find(s, pos + k)
    print(sir)


def prob4():
    cuv1 = "emerit"
    cuv2 = "treimi"
    for c in cuv1:
        if cuv2.find(c) == -1:
            print("nu sunt anagrame")
            break
        else:
            cuv2 = cuv2.replace(c, '', 1)
    else:
        print("sunt anagrame")


def prob5():
    k = 1
    text = "un text interesant"
    text2 = "abcd efg z"
    cript = list(text2)
    for i in range(0, len(cript)):
        if cript[i] != ' ':
            c = ord(cript[i])
            c -= ord('a')
            c = (c + k) % 26
            c += ord('a')
            cript[i] = chr(c)
    print(''.join(cript))


def prob6():
    text = """Astăzi am cumpărat pâine de 5 RON, pe lapte am dat 10 RON, iar de 15 RON am cumpărat niște
cașcaval. De asemenea, mi-am cumpărat și niște papuci cu 50 RON!"""
    text.replace(',', ' ')
    text.replace('!', ' ')
    cuvs = text.split(' ')
    suma=0
    for i in range(0, len(cuvs)):
        if cuvs[i] == 'RON':
            suma += int(cuvs[i-1])
    print(suma)
