
# coding: utf-8
import fileinput
import re
import os
import csv


TWEET_TRAIN_FILE_NAME = "tweet_train.arff"
TWEET_TRAIN_FILE_NAME_EXIT = "tweet_train_tt.arff"
ANTIDICTIONARRY_FILE = "antidico.txt"

#Antodictionnary treatment : remove all the word who are defined as "common" and arnt meaningful
a_dict = []
with open(ANTIDICTIONARRY_FILE, "r") as f:
    for l in f :
        a_dict.append(l.strip('\n'))

noise = '|'.join(a_dict)
pattern = re.compile("\\b("+ noise +")\\W",re.I)


with open('polarimots_1.csv', mode='r') as infile:
    reader = csv.reader(infile)
    polarimots = dict((rows[0],rows[1:4]) for rows in reader)



#Main work
fexit = open(TWEET_TRAIN_FILE_NAME_EXIT, "w+")
with open(TWEET_TRAIN_FILE_NAME, "r") as f:
    for l in f:
        if l[0] != '@' :
            symbol = l[-2:]    #the last char is the carriage return one, the antepenultimate is the symbol
            text = l.split('",')[0]

            #antidictionnary treatment
            text = pattern.sub("",text)


            #accent treatment
            text = re.sub("é|è|ê","e",text)
            text = re.sub("à|â","a",text)
            text = re.sub("î","i",text)
            text = re.sub("ô","o",text)
            text = re.sub("ù|û","u",text)
            text = re.sub("ç","c",text)
            

            text = text.lower()
            text = re.sub("(@\w+)","<utilisateur>",text) #replace @people with a <utilisateur> tag
            text = re.sub("\d+[., ]\d+","<nombre>",text) #replace any number with a <nombre> tag
            text = re.sub("(http://[^\s]+)|(https://[^\s]+)","<url>",text) #replace any url with a <url> tag
            #replace any time period with the symbol <duree> (maybe need improvement for the s at the end)
            text = re.sub("<nombre>(\s?minutes|\s?heures|\s?secondes|\s?jours|\s?semaines|\s?années|min|sec|h)","<duree>",text) 

          
            
            l = text + '",' + l.split('",')[1]
        fexit.write(l)

        


#os.system(s)


#s = 'echo ' + text + '" | $LIA_TAGG/script/lia_clean | $LIA_TAGG/bin/lia_rm_ponct'
#print s
#r = os.popen(s).read()
#print r.strip('\n')
            