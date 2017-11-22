
# coding: utf-8

import re

ANTIDICTIONARRY_FILE = "antidico.txt"

a_dict = []
with open(ANTIDICTIONARRY_FILE, "r") as f:
    for l in f :
        a_dict.append(l.strip('\n'))

noise = '|'.join(a_dict)


s = "ceci est un exemple de texte tout nul avec des mots qui serons suprimmés"

pattern = re.compile("\\b("+ noise +")\\W",re.I)

print pattern.sub("",s)