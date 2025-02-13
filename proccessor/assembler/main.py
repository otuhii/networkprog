#for simple arm hello world program
from ops import dpis, sdts, regs, swis
from dataclasses import dataclass
from typing import Optional, Union, List
import re

#garbage arithmetic
#todo: fix offsets, fix arithmetic label function, convertation to binary 



@dataclass
class Label:
    name: Optional[str] = None
    size : int = 0
    data : Optional[Union[int, List[int]]] = None
    offset : int = 0
    isGlobal : bool = False
    section : Optional[str] = None

@dataclass
class Section:
    name : Optional[str] = None
    startOffset : Optional[int] = None
    size : int = 0


class Assembler:
    def __init__(self, filename):
        self.labels = []
        self.sections = []

        self.offset = 0

        self.currentLabel = None
        self.currentSection = None
        self.INSTRUCTION_SIZE = 4 
        
        with open(filename, "r") as f:
            self.code = f.readlines()
        


    def processSectionsFP(self, line):
        sectionName = line.split(".")[1]
        if 'ascii' not in line:
            if "global" in sectionName: #then its not section
                globalLabelName = sectionName.split(" ")[1]           
                for label in self.labels:
                    if label.name == globalLabelName:
                        label.isGlobal = True
                        return
                self.labels.append(Label(name=globalLabelName, isGlobal=True))
            else:
                if len(self.sections) != 0:
                    for sec in self.sections:
                        if sec.name == sectionName:
                            print("error! section with this mame already exists")
                            return
                self.sections.append(Section(sectionName, self.offset))
                

    def processSectionsSP(self, line):
        sectionName = line.split('.')[1]
        for i, section in enumerate(self.sections):
            if section.name == sectionName:
                self.currentSection = i


    
    def processLabelFP(self, line):
        labelName = line.split(":")[0].strip()
        for label in self.labels:
            if label.name == labelName:
                label.offset = self.offset
                return
        self.labels.append(Label(name=labelName, offset=self.offset))


        return 

    def processLabelSP(self, line):
        labelName = line.split(":")[0].strip()
        for i, label in enumerate(self.labels):
            if label.name == labelName:
                self.currentLabel = i


    def firstPass(self):
        print("FIRST PASS")
        for line in self.code:
            print(f" {self.offset} : {line}\n\n")
            
            line = line.strip("\n").strip()
                
            if line.startswith("."):
                self.processSectionsFP(line)
                continue

            if line.endswith(":"):
                self.processLabelFP(line)
                continue
            

    def secondPass(self):
        print("===========================================")
        print("SECOND PASS")
        for line in self.code:
            print(f" {self.offset} : {line}\n\n")

            if 'ascii' in line:
                self.processAscii(line)
                continue

            line = line.strip("\n").strip()

    
            if '= . -' in line:
                self.processAssign(line)
                continue
                
            if line.startswith("."):
                self.processSectionsSP(line)
                continue

            if line.endswith(":"):
                self.processLabelSP(line)
                continue

            for op in dpis:
                if op in line:
                    self.processDPI(line)
            
            for op in sdts:
                if op in line:
                    self.processSDT(line)

            for op in swis:
                if op in line:
                    self.processSWI(line)




    def parseFile(self):
        self.firstPass()

        self.offset = 0
        self.currentSection = None
        self.currentLabel = None

        self.secondPass()

    
    #second pass
    def processAscii(self, line: str):
        match = re.search(r'"([^"]*)"', line) #some fancy regex string detection
        if not match:
            raise SyntaxError(f"Invalid ASCII string format in line: {line}")
        
        try:
            string = bytes(match.group(1), 'utf-8').decode('unicode-escape').encode('ascii')
            asciiData = list(string)
        except UnicodeError as e:
            raise ValueError(f"Invalid characters in ASCII string: {e}")
        
        # Find all labels at current offset
        currentLabelsIdxs = [
            i for i, label in enumerate(self.labels)
            if label.offset == self.offset
        ]
    
        if not currentLabelsIdxs:
            raise ValueError(f"No label found for ASCII data: {line}")
    
        padding = (4 - (len(asciiData) % 4)) % 4
        alignedAsciiData = asciiData + [0] * padding

        for idx in currentLabelsIdxs:
            self.labels[idx].data = alignedAsciiData
            self.labels[idx].size = len(asciiData)
        
        self.offset += len(alignedAsciiData)



    #for var = . - var1 calculation
    #fix hardcoded . symbol
    #i should definitely rewrite this function
    def processAssign(self, line):
        line = line.split(" ")
        label = line[0]
        result = None
        label2Name = line[-1]
        for l in self.labels:
            if l.name == label2Name:
                #its actually really dumb and unconvenient but idgaf
                result = l.size - l.offset
        
        if result != None:
            self.labels.append(Label(label, data=result))
        else:
            print("error processing assigment")
            return


    def processDPI(self, line):
        self.offset += self.INSTRUCTION_SIZE
        self.sections[self.currentLabel].size += self.INSTRUCTION_SIZE
    def processSDT(self, line):
        self.offset += self.INSTRUCTION_SIZE
        self.sections[self.currentLabel].size += self.INSTRUCTION_SIZE
    def processSWI(self, line):
        self.offset += self.INSTRUCTION_SIZE
        self.sections[self.currentLabel].size += self.INSTRUCTION_SIZE


    def debug(self):
        print(self.sections)
        print(self.labels)
            


if __name__ == "__main__":
    ass = Assembler("hw.s");
    ass.parseFile()
    ass.debug() #i need more asses
