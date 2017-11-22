import fileinput
import re

TWEET_TRAIN_FILE_NAME = "tweet_train.arff"
TWEET_TRAIN_FILE_NAME_EXIT = "tweet_train_tt.arff"

fexit = open(TWEET_TRAIN_FILE_NAME_EXIT, "w+")
with open(TWEET_TRAIN_FILE_NAME, "r") as f:
    for l in f:
        if l[0] != '@' :
            symbol = l[-2:]    #the last char is the carriage return one, the antepenultimate is the symbol
            text = l.split('",')[0]

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

        


