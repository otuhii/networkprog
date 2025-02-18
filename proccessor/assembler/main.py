#for simple arm hello world program
from ops import dpis, sdts, regs, swis
from dataclasses import dataclass
from typing import Optional, Union, List, Dict
import re
import struct

#no conditions and i dont give a fuck for now
#i think it is the worst assembler in this world, when i wrote first pass i had no idea what do i actually need so maybe there is some non-sense parts

@dataclass
class Label:
    name: Optional[str] = None
    size : int = 0
    data : Optional[Union[int, List[int]]] = None
    offset : Optional[int] = None
    isGlobal : bool = False
    section : Optional[str] = None
    padding : int = 0
    isConstant : bool = False
    literalPoolAddress : Optional[int] = None


@dataclass
class Section:
    data : Dict[int, Union[str, int]]
    name : Optional[str] = None
    startOffset : Optional[int] = None
    size : int = 0
    padding : int = 0 #update on end of section
    baseAddress : int = 0
    poolEntry : Optional[int] = None   

#i could do labels and sections not array but dict and it would be more efficient i guess but i don't have much desire to rewrite it
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
        

        # offset : [normal representation, binary(or hex or whatever)]
        self.processedCode = {}

        self.literalPool = []



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
                padding=0,
                baseAddress = 0x1000 if sectionName=='text' else 0x2000,
                data={}
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



    #addressing and stuff
    def firstPass(self):
        for line in self.code:
            line = line.strip("\n").strip()
            
            self.processedCode[self.offset] = line
            
            currentOffset = self.offset 

            if 'ascii' in line:
                self.processAscii(line)
                continue    
            
            if not line:
                continue

            elif '=' in line and ". -" in line:
                self.processAssign(line)
                continue

            elif line.startswith("."):
                self.processSections(line)
                continue

            elif line.endswith(":"):
                self.processLabel(line)
                continue
            else:
                if '=' in line:
                    labelName = line.split()[-1].split('=')[1]
                    if labelName not in self.literalPool:
                        self.literalPool.append(labelName)

                if self.currentSection is not None:
                    self.sections[self.currentSection].size += self.INSTRUCTION_SIZE
                    section = self.sections[self.currentSection]
                    section.data[self.offset-section.startOffset] = line
                self.offset += self.INSTRUCTION_SIZE



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
                        section=self.sections[self.currentSection],
                        offset = self.offset,
                        isConstant = True
                    ))
                    return
                    
            raise ValueError(f"Referenced label {labelName} not found")

##################################################################################
##################################################################################
##################################################################################


    def tokenize(self):
        self.code = None
        self.tokenizeLabelData()
        startPoint = None
        self.literalPoolAddressing()

        for label in self.labels:
            if 'start' in label.name:
                startPoint = label.offset
                break


        textSection = next(s for s in self.sections if s.name == 'text')
        for offset, line in textSection.data.items():
            
            if offset >= textSection.poolEntry:
                break

            self.currentSection = 1

            if not isinstance(line, int):
                tokens = line.split()
            for idx in range(len(tokens)):
                tokens[idx] = tokens[idx].split(",")[0]

            operands = tokens[1:]
            instruction = tokens[0]
            if instruction in dpis:  
                machine_code = self.DPItoBIN(instruction, operands, offset)
                print(f"{tokens} --> {machine_code}")
                continue
            elif instruction in sdts:  
                machine_code = self.SDTtoBIN(instruction, operands, offset)
                print(f"{tokens} --> {machine_code}")
                continue
            elif instruction in swis:  
                machine_code = self.SWItoBIN(instruction, operands, offset)
                print(f"{tokens} --> {machine_code}")
                continue
            
        
        #self.writeToBinary()




    def DPItoBIN(self, instruction, operands, offset, condition=0b1110, flags=0b0):
        rd = int(operands[0][1:])
        operand2 = operands[1]

        if operand2.startswith('#'):
            immVal = int(operand2[1:])

            rotate, imm8 = self.encodeImm(immVal)
            operand2_code = (rotate << 8) | imm8
            I = 0b1 #immediate operand
        
        else:

            raise NotImplementedError("Register operands not implemented")

        opcode = dpis[instruction]


        binary = ( 
            (condition << 28) |
            (I << 25) |
            (opcode << 21) | 
            (flags << 20) |
            (rd << 12) |
            operand2_code
        )
        return struct.pack('<I', binary)




    def SDTtoBIN(self, instruction, operands, offset, condition=0b1110):
        if instruction == 'ldr':
            # Get target register
            rd = int(operands[0][1:])
            opcode = sdts[instruction]
            # Get PC-relative offset info
            pc, offset = self.processPseudoInst(offset, operands[1].split('=')[1])
            
            return struct.pack('<I', 
                (condition << 28) |
                (opcode << 25) |    # I=0 for immediate offset
                (1 << 24) |       # P=1 (offset addressing)
                (1 << 23) |  # U=1 for positive offset
                (0 << 22) |       # B=0 (word transfer)
                (0 << 21) |       # W=0 (no writeback)
                (1 << 20) |       # L=1 (load)
                (0b1111 << 16) |  # Rn=PC (0b1111)
                (rd << 12) |
                (offset & 0xFFF)  # Use absolute offset value
            )


    def SWItoBIN(self, instruction, operands, offset, condition=0b1110, flags=0b0):
        imm24 = int(operands[0].strip('# '))
        binary = (condition << 28) | (0b1111 << 24) | (imm24 & 0x00FFFFFF)

        return struct.pack('<I', binary)


    def tokenizeLabelData(self):
        for label in self.labels:
            if isinstance(label.data, str):
                hex_bytes = [hex(ord(c)) for c in label.data]
                label.data = hex_bytes



    #convert label to [pc, offset]
    def processPseudoInst(self, currentOffset, labelName):
        label = next(l for l in self.labels if l.name == labelName)
        pc = currentOffset + 8
        offset = label.literalPoolAddress - pc

        return [pc, offset]

    def literalPoolAddressing(self):
        textSection = next(s for s in self.sections if s.name == 'text')
        textSection.poolEntry = textSection.size

        for labelName in self.literalPool:
            label = next(l for l in self.labels if l.name == labelName)
            if not label.isConstant:
                address = label.section.baseAddress+label.offset
                textSection.data[textSection.size] = address
            else:
                textSection.data[textSection.size] = label.data

            label.literalPoolAddress = textSection.size
            textSection.size += 4

    def packELF(self):
        pass

    

    def writeToBinary(self, name="out.o"):
        elfStruct = self.packELF()
        f = open(name, "a")
        f.write(elfStruct)
        f.close()        



    def encodeImm(self, value):
    #some cool stuff that i have no clue about
    #ARM immediate encoding: 8-bit value rotated right by even bits.
        for rotate in range(0, 16):
            imm8 = (value >> (2 * rotate)) & 0xFF
            if (imm8 << (2 * rotate)) == value:
                return (rotate, imm8)
        raise ValueError(f"Cannot encode immediate {value}")



##################################################################################
##################################################################################
##################################################################################

    def parseFile(self):
        self.firstPass()

        self.offset = 0
        self.currentSection = None
        self.currentLabel = None

        self.tokenize()


    def debug(self):
        print(self.processedCode)
        print(self.sections)
        print(self.labels)
        print(self.literalPool)
        textSection = next(s for s in self.sections if s.name == 'text')
        for of, l in textSection.data.items():
            print(f"{of} : {l}")


if __name__ == "__main__":
    fatass = Assembler("hw.s");
    fatass.parseFile()
    fatass.debug() 
