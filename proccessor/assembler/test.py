from ops import dpis, sdts, regs, swis

def DPItoBIN(instruction, operands):
    print(f"ins: {instruction} for oper: {operands}")

if __name__ == "__main__":
    DPItoBIN("mov", ['r0', '#1'])
