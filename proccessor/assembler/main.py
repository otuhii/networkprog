#for simple arm hello world program

ops = {
        'mov' : '0001 101S 0000 dddd ssss sss0 mmmm',
        'svc' : '1110 1111 0000 0000 0000 0000 0000 0000',
        'ldr' : '0100 U001 nnnn dddd iiii iiii iiii',
}

regs = {
    'r0' : '0000',
    'r1' : '0001',
    'r2' : '0010',
    'r3' : '0011',
    'r4' : '0100',
    'r5' : '0101',
    'r6' : '0110',
    'r7' : '0111',
    'r8' : '1000',
    'r9' : '1001',
    'r10' : '1010',
    'r11' : '1011',
    'r12' : '1100',
    'r13' : '1101',#stack pointer 
    'r14' : '1110',#link register tf is it xd
    'r15' : '1111',#program counter
} 




class Assembler:
    def __init__(self, filename):
        self.labels = {}
        self.sections = {}
        
        #section syntax - name : {
        #                        size : 00,
        #                        isGlobal : 11
        #                       }


        self.offset = 0
        
        with open(filename, "r") as f:
            self.code = f.readlines()
        


    def processSections(self, line):
        line = line.strip("\n").strip()
        sectionName = line.split(".")[1]
        if "global" in sectionName: #then its not section
            labelName = sectionName.split(" ")[1]
            if labelName in self.labels:
                self.labels[labelName]["isGlobal"] = 1 
            else:
                self.labels[labelName] = {
                            "offset" : None,
                            "isGlobal" : 1
                        }
        else:
            self.sections[sectionName] = bin(self.offset)


        self.offset += 4
        return
    

    
    def processLabel(self, line):
        line = line.strip("\n").strip()
        labelName = line.split(":")[0]
        if labelName in self.labels: 
            if self.labels[labelName]["offset"] is not None:
                print("error!!") # handle this appropriately in future
            else:
                self.labels[labelName]["offset"] = self.offset

        else:
            self.labels[labelName] = {
                        "offset" : self.offset,
                        "isGlobal" : 0
                    }
            

        self.offset += 4
        return



    def parseFile(self):
        for line in self.code:
            #print(f"{self.offset}:{line}")
            if line.startswith("."):
                self.processSections(line)
            if line.endswith(":\n"):
                self.processLabel(line)
            

            


    def debug(self):
        print(self.sections)
        print(self.labels)
            


if __name__ == "__main__":
    ass = Assembler("hw.s");
    ass.parseFile()
    ass.debug() #i need more asses
