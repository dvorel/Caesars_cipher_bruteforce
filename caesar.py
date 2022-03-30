import re




class Caesar():
    def __init__(self):
        self.dict = {" ": " "}  #konstanta
        
    def wordsToKey(self, wordIn, wordOut):
        #pretpostavljamo len wordin = len word out
        wordOut = wordOut.lower()
        
        for i in range(len(wordIn)):
            if wordOut[i] in self.dict.keys():
                if self.dict[wordOut[i]] != wordIn[i]:
                    return True
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
    
    
def inDict(word, dict):
    for w in dict:
        if word == w:
            return True
    return False

if __name__ == "__main__":   
    wordDict = ["rakdgtzpća", "uftđbzpi", "laž"]
    wordEnc = "utzpćbdž"
    
    matching = {}
    caesarsList = []
    generated = []
    
    #read dict from file
    dict = []
    f = open("dict/hr.txt", "r", encoding="utf-8")
    for line in f:
        dict.append(line.split("\t")[0])
        
    #find words matching length    
    for e in wordDict:
        l = []
        for d in dict:
            if len(e) == len(d):
                l.append(d)
        
        #add list of matching to dict
        matching[e] = l
    
    w1, w2, w3 = list(matching.keys())

    for d1 in matching[w1]:
        caesar = Caesar()
                        
        if not caesar.wordsToKey(w1, d1):
            continue
        
        for d2 in matching[w2]:
            if not caesar.wordsToKey(w2, d2):
                continue
            
            
            for d3 in matching[w3]:
                #generate dict for decrypt
                if not caesar.wordsToKey(w3, d3):
                    continue
                
                #decrypt word
                d = caesar.decrypt(wordEnc)
                
                if inDict(d, dict):
                    print("Decrypted: ", d)
                    print("Key: ", caesar.dict)
                    
                    #append to lists
                    caesarsList.append(caesar)
                    generated.append(d)
                    
                