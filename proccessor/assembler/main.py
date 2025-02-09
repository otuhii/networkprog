#for simple arm hello world program

ops = {
        'mov' : '0001 101S 0000 dddd ssss sss0 mmmm',
        'svc' : '1110 1111 0000 0000 0000 0000 0000 0000'
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




def ascii(line):
    



