#for simple arm hello world program
from ops import dpis, sdts, regs, swis


#garbage arithmetic
#todo: fix offsets, fix arithmetic label function, convertation to binary 


class Assembler:
    def __init__(self, filename):
        self.labels = {}
        self.sections = {}
        
        self.currentLabel = None
        self.currentSection = None

        #section syntax - name : {
        #                        size : 00,
        #                        isGlobal : 11
        #                       }


        self.offset = 0
        
        with open(filename, "r") as f:
            self.code = f.readlines()
        


    def processSections(self, line):
        sectionName = line.split(".")[1]
        if "global" in sectionName: #then its not section
            labelName = sectionName.split(" ")[1]
            if labelName in self.labels:
                self.labels[labelName]["isGlobal"] = 1 
            else:
                self.labels[labelName] = {
                            "data" : None,
                            "offset" : None,
                            "isGlobal" : 1
                        }
        else:
            self.currentSection = sectionName   
            self.sections[sectionName] = bin(self.offset)


        self.offset += 4
        return
       

    
    
    def processLabel(self, line):
        labelName = line.split(":")[0]
        self.currentLabel = labelName
        if labelName in self.labels: 
            if self.labels[labelName]["offset"] is not None:
                print("error!!") # handle this appropriately in future
            else:
                self.labels[labelName]["offset"] = self.offset

        else:
            self.labels[labelName] = {
                        "data" : None,
                        "offset" : self.offset,
                        "isGlobal" : 0
                    }
            

        self.offset += 4
        return 
    

    def processLabelArith(self, line):
        line = line.split(" ")
        label = line[0]
        l = line[-1]
        if self.labels[l]["data"] is not None:
            #if self.labels[l]["data"] is list: #lets say that list always means ascii
            self.labels[label] = self.offset - len(self.labels[l]["data"])
            #i can add some values support but idk 
        


    
    def processDPI(self, line):
        tokens = line.split(" ")



    def processSDT(self, line):
        tokens = line.split(" ")



    def processAscii(self, line):
        string = line.split(" ")[-1]
        asciiString = list(string.encode('ascii'))

        if self.labels[self.currentLabel]["data"] != None:
            print("error") #handle this appropriately in future
            return
        else:
            self.labels[self.currentLabel]["data"] = asciiString;
        

        self.offset += len(asciiString)
        return


    def parseFile(self):
        for line in self.code:
            line = line.strip("\n").strip()
                
            if 'ascii' in line:
                self.processAscii(line)
                continue
            
            if ". - " in line:
                self.processLabelArith(line)
                continue

            if line.startswith("."):
                self.processSections(line)
                continue

            if line.endswith(":"):
                self.processLabel(line)
                continue

            for op in dpis:
                if op in line:
                    self.processDPI(line)
                    continue
            
            for op in sdts:
                if op in line:
                    self.processSDT(line)
                    continue
            
            print(f"{self.currentSection} {self.currentLabel} || {self.offset} : {line}\n\n")


    def debug(self):
        print(self.sections)
        print(self.labels)
            


if __name__ == "__main__":
    ass = Assembler("hw.s");
    ass.parseFile()
    ass.debug() #i need more asses
