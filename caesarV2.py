import threading



class Caesar():
    def __init__(self):
        self.dict = {}  #konstanta
        
    def wordsToKey(self, wordIn, wordOut):
        #pretpostavljamo len wordin = len word out
        wordOut = wordOut.lower()
        
        for i in range(len(wordIn)):
            if wordOut[i] in self.dict.keys():
                if self.dict[wordOut[i]] != wordIn[i]:
                    continue
                
            elif wordIn[i] in self.dict.values():
                key = [k for k, v in self.dict.items() if v ==wordIn[i]][0]
                if key != wordOut[i]:
                    return False
                
            self.dict[wordOut[i]] = wordIn[i]
            
        return True                
        
        
    def encrypt(self, word):
        default = {self.input[i]: self.output[i] for i in range(len(self.input))}
        encWord = ""
        for i in range(len(word)):
            if word[i] in self.dict.keys():
                encWord += self.dict[word[i]]
            else:
                encWord += default[word[i]]
        
        return encWord
    
    def decrypt(self, word):
        decWord = ""
        for i in range(len(word)):
            if word[i] in self.dict.values():
                key = [k for k, v in self.dict.items() if v == word[i]][0]
                decWord += key
                        
        return decWord
    

def checkWord(w1, d1, w2, d2):
    global caesarKeys, encrypted, printing, dicts
    
    caesar = Caesar()

    #key
    if not caesar.wordsToKey(w1, d1):
        return

    if not caesar.wordsToKey(w2, d2):
        return
    
    
    #decr
    for word in encrypted:
        if word in (w1+w2) and len(word)>3:
            d = caesar.decrypt(word)
            
            if not d in dicts:                   
                del caesar
                return
    
    if w1 in caesarKeys.keys():
        del caesarKeys[w1]
    
    key = ""
    val = ""
    
    for k, v in caesar.dict.items():
        key += str(k)
        val += str(v)
    
    caesarKeys[key] = val
    
    del key
    del val
    del caesar
    #print("VAL: ", val)
    
    # while printing:
    #     pass
    
    # printing = True
    # print("\nW: ", w1, " | ", w2)
    # print("Key: ", key)
    # print("Val: ", val)
    # printing = False
    
    

#globals
printing = False
dicts = []
dict = []
encrypted = []
caesarKeys = {" ": " "}



if __name__ == "__main__":

    
    #read dictionary from txt    
    f = open("dict/hr.txt", "r", encoding="utf-8")
    for line in f:
        dict.append(line.split("\t")[0])
    dicts = set(dict)
    
    #read encrypted txt from file
    e = open("rtgde_zp_lab.txt", "r", encoding="utf-8")
    for line in e:
        for w in line.split(" "):
            encrypted.append(w)
    
    
    n = 0
    
    #iterate
    while n < len(encrypted):
        #reset used vars
        threads = []
        matching = {}
        
        word = encrypted[n]
        n+=1
        
        usedKeys = caesarKeys.copy()
        
        #find all new words in dict
        dictWords = []
        for d in dict:
            if len(word) == len(d):
                dictWords.append(d)
        
        for k, v in usedKeys.items():
            print(f"N:{n} | K:{k} | V:{v}")
            for d in dictWords:
                
                if len(threads) > 30000:
                    print("MAX THREADS!")
                    for t in range(len(threads)):
                        threads[t].join()
                    threads = []
                    print("ALL THREADS FINISHED!")
                        
                #decrypt word
                threads.append(threading.Thread(target=checkWord, args=(k, v, word, d)))
                threads[-1].start()
        
        
        del usedKeys
        del dictWords
        
        for thr in threads:
            thr.join()
        
        print("Iter: ", n)        
        for k, v in caesarKeys.items():
            print(f"Key:{k} | Val:{v}")
        print("LEN: ", len(caesarKeys))
