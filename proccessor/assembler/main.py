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
    offset : Optional[int] = None
    isGlobal : bool = False
    section : Optional[str] = None
    padding : int = 0
    endoffset : Optional[int] = None

@dataclass
class Section:
    name : Optional[str] = None
    startOffset : Optional[int] = None
    size : int = 0
    padding : int = 0 #update on end of section

class Assembler:
    def __init__(self, filename):
        self.labels = []
        self.sections = []

        self.offset = 0
        self.INSTRUCTION_SIZE = 4
        
        self.currentLabel = None
        self.currentSection = None

        with open(filename, "r") as f:
            self.code = f.readlines()
        


    def processSections(self, line):
        sectionName = line.split(".")[1]
        
        if "global" in sectionName: #then its not section
            globalLabelName = sectionName.split(" ")[1]           
            for label in self.labels:
                if label.name == globalLabelName:
                    label.isGlobal = True
                    return
            self.labels.append(Label(name=globalLabelName, isGlobal=True))
        else:
            if self.currentSection is not None:
                current = self.sections[self.currentSection]
                current.endOffset = self.offset
                current.size = current.endOffset - current.startOffset
                self.offset += current.padding  
            
            for sec in self.sections:
                if sec.name == sectionName:
                    self.currentSection = self.sections.index(sec)
                    return
            
            self.sections.append(Section(
                name=sectionName,
                startOffset=self.offset,
                padding=0
            ))
            self.currentSection = len(self.sections) - 1


    
    def processLabel(self, line):
        labelName = line.split(":")[0].strip()
        
        existing_label = None
        for idx, lab in enumerate(self.labels):
            if lab.name == labelName:
                if lab.section and lab.section.name == self.sections[self.currentSection].name:
                    print(f"erorr multiple label declaration with same name{labelName}")
                    return
                elif lab.isGlobal:
                    existing_label = idx

        if existing_label is not None:
            self.labels[existing_label].offset = self.offset
            self.labels[existing_label].section = self.sections[self.currentSection]
            self.currentLabel = existing_label

        else:
            self.labels.append(Label(
                name=labelName,
                offset=self.offset,
                section=self.sections[self.currentSection]
                ))
            self.currentLabel = len(self.labels)-1



    def firstPass(self):
        print("FIRST PASS")
        for line in self.code:
            print(f" {self.offset} : {line}\n\n")

            if 'ascii' in line:
                self.processAscii(line)
                continue
                
            line = line.strip("\n").strip()
            
            if not line:
                continue

            if '=' in line and ". -" in line:
                self.processAssign(line)
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
            
            for op in sdts:
                if op in line:
                    self.processSDT(line)

            for op in swis:
                if op in line:
                    self.processSWI(line)


    def secondPass(self):
        for line in self.code:
            print(f" {self.offset} : {line}\n\n")

            line = line.strip("\n").strip()

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



    def processAscii(self, line):
        match = re.search(r'"([^"]*)"', line) #some fancy regex string detection
        if not match:
            raise SyntaxError(f"Invalid ASCII string format in line: {line}")
        
        try:
            word = match.group(1)
            word = bytes(word, 'utf-8').decode('unicode-escape')
            size = len(word)

            self.sections[self.currentSection].size += size
            self.labels[self.currentLabel].size = size
            self.labels[self.currentLabel].data = word
            self.offset += size


            padding = (4-(size%4)) % 4
            if padding:
                self.sections[self.currentSection].padding += padding
                self.labels[self.currentLabel].padding += padding

        except UnicodeError as e:
            raise ValueError(f"Invalid characters in ASCII string: {e}")
    


    #only for . - var format   
    def processAssign(self, line):
        line = line.split("=")
        if len(line) != 2:
            raise SyntaxError("!!!!")


        assignTo = line[0].strip()
        expression = line[1].strip()
        if ". -" in expression:
            labelName = expression.split("-")[1].strip()
            
            for label in self.labels:
                if label.name == labelName:
                    result = self.offset - label.offset
                    self.labels.append(Label(
                        name=assignTo,
                        data=result,
                        section=self.sections[self.currentSection]
                    ))
                    return
                    
            raise ValueError(f"Referenced label {labelName} not found")

    


    def parseFile(self):
        self.firstPass()

        self.offset = 0
        self.currentSection = None
        self.currentLabel = None

        #self.secondPass()



    def processDPI(self, line):
        self.offset += self.INSTRUCTION_SIZE
        self.sections[self.currentSection].size += self.INSTRUCTION_SIZE
    def processSDT(self, line):
        self.offset += self.INSTRUCTION_SIZE
        self.sections[self.currentSection].size += self.INSTRUCTION_SIZE
    def processSWI(self, line):
        self.offset += self.INSTRUCTION_SIZE
        self.sections[self.currentSection].size += self.INSTRUCTION_SIZE


    def debug(self):
        print(self.sections)
        print(self.labels)
            


if __name__ == "__main__":
    fatass = Assembler("hw.s");
    fatass.parseFile()
    fatass.debug() 
