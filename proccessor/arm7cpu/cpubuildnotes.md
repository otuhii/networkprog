some my thoughts notes and questions/answers will be here along my cpu building way


register - combination of small latches that are combination of transistor assembled in the way to store or read some signal

ram - stored as list of matrices because it is more efficient to address

sram - fast but expensive 
dram - slower but less expensive(has smaller number of transistors, because it uses not those logic gates but specific transistor mosfet and capacitor)

--- whole series of this videos is amazing 
https://www.youtube.com/watch?v=GYlNoAMBY6o&list=PL9vTTBa7QaQOoMfpP3ztvgyQkPWDPfJez&index=4
---


 !BEST resources for learning for now:
    --https://github.com/hughperkins/cpu-tutorial - for now its the best guy i saw so i would follow his guide
    --https://en.wikipedia.org/wiki/Classic_RISC_pipeline

cache - sram(here speed matters more)
regular ram - dram

jmp - core concept of this inst is just overwriting address register


while(a<5) is performed as subtraction(mindblowing) a - 5, if value is negative then its true, if vvalue is 0 then  a == 5 and if value is potitive then a > 5


clock values only important on its change(pos edge, neg edge) because when value is constant nothing happens

timings in this all this things are so important

fetch/decode/execute stages are conducted by sequence of jk-flip-flops(each of flip-flops divides signal frequency by 2 and it makes easy to switch between stages)

steve mould shows some cool explanation of these flip-flops

turing completeness - capacity of certain system to perform complex tasks(it must support conditions, it needs to be able to perform iteration and recursion, it must be able to read and write to memory) 



harvard vs von neumann archtecture
  --harvard: 
      instructions and data stored separately, separate buses
      faster because of this parallelism
      safer
      more expensive
      less flexible

  --von neumann:
      instruction and data stored in same data space, single bus
      no parallel execution
      easier to create
      flexible

  -- modified harvard:
      separate caches but same space for "simple" data and instruction
      involves productivity of harvard arch and flexibility of VN arch

      -hard to keep caches in order
      -some other problems with caches


block ram(bram):
  local fpga toy, stores large amount of data inside fpga







### **What You Need to Learn/Review**
#### **1. ARM7TDMI Architecture Basics**
   - **Key Features**:
     - 32-bit RISC architecture.
     - 3-stage pipeline: **Fetch → Decode → Execute**.
     - 37 registers (including banked registers for modes like IRQ/SVC).
     - CPSR (Current Program Status Register) for flags (N, Z, C, V) and -b
   - **Critical Resource**: Study the [ARM7TDMI Technical Reference Manual](https://developer.arm.com/documentation/ddi0210/c/).
   

#### **2. Digital Logic Fundamentals**
   - **Core Components**:
     - **Register File**: 16 general-purpose registers (R0-R15, where R15 = PC).
     - **ALU**: Arithmetic/Logic Unit (supports ADD, SUB, AND, OR, etc.).
     - **Control Unit**: Decodes instructions and generates control signals.
     - **Memory Interface**: Connects to instruction/data memory (SRAM/BRAM).
   - **Key Concepts**:
     - Clock cycles and pipeline stalls.
     - Data hazards (e.g., read-after-write dependencies).

#### **4. Memory Subsystem**
   - **Harvard vs. Von Neumann Architecture**:
     - ARM7 uses Von Neumann (shared instruction/data memory).
   - **Memory-Mapped I/O**:
     - How peripherals (e.g., UART) interact with the CPU.


- **Verilator**: Simulate your CPU and catch logic errors early.
- **GDB/OpenOCD**: If testing on FPGA later.
- **Waveform viewers (GTKWave)**: Debug pipeline stages.

### **Common Pitfalls to Avoid**
1. **Pipeline Stalls**: Ignoring data hazards (e.g., using a register before it’s written).
2. **Endianness**: ARM is little-endian; ensure memory reads/writes match.
3. **Branch Handling**: Forgetting to update PC for `b` or `bl` instructions.












registers and shit from perplexity, possible that it is invalid
The ARM7TDMI processor has a total of 37 registers, consisting of 31 general-purpose 32-bit registers and six status registers. However, not all these registers are accessible at the same time; their availability depends on the processor's state and operating mode. Here's a breakdown of the registers:

### General-Purpose Registers
- **R0 to R7**: These are unbanked registers, meaning they are the same across all modes.
- **R8 to R12**: These are banked registers, which means they have different versions depending on the mode.
- **R13**: Used as a stack pointer in most modes. It is banked across modes.
- **R14**: Used as a link register. It is banked across modes.
- **R15**: The Program Counter (PC).

### Banked Registers
- **R8_fiq to R12_fiq**: Banked registers for FIQ mode.
- **R13_svc, R14_svc**: Banked registers for Supervisor mode.
- **R13_irq, R14_irq**: Banked registers for IRQ mode.
- **R13_abt, R14_abt**: Banked registers for Abort mode.
- **R13_und, R14_und**: Banked registers for Undefined mode.

### Status Registers
- **CPSR (Current Program Status Register)**: Contains condition code flags and the current mode bits.
- **SPSR_fiq, SPSR_svc, SPSR_abt, SPSR_irq, SPSR_und**: Saved Program Status Registers for each mode.

Here is a more detailed list of all 37 registers, considering the banked registers:

1. **R0**
2. **R1**
3. **R2**
4. **R3**
5. **R4**
6. **R5**
7. **R6**
8. **R7**
9. **R8**
10. **R9**
11. **R10**
12. **R11**
13. **R12**
14. **R13**
15. **R14**
16. **R15 (PC)**
17. **R8_fiq**
18. **R9_fiq**
19. **R10_fiq**
20. **R11_fiq**
21. **R12_fiq**
22. **R13_svc**
23. **R14_svc**
24. **R13_irq**
25. **R14_irq**
26. **R13_abt**
27. **R14_abt**
28. **R13_und**
29. **R14_und**
30. **CPSR**
31. **SPSR_fiq**
32. **SPSR_svc**
33. **SPSR_abt**
34. **SPSR_irq**
35. **SPSR_und**
36. **SP_fiq** (Implicitly banked with R13_fiq)
37. **SP_svc, SP_irq, SP_abt, SP_und** (Implicitly banked with R13 in each mode)

Note: The stack pointers (SP) for each mode are often considered part of the banked R13 registers, but they are not explicitly listed as separate registers in some documentation. However, they are implicitly banked across modes.

The ARM7TDMI's register set is designed to optimize performance by minimizing the need to save and restore registers during mode changes, especially during exception handling[1][2][3][4].

Citations:
[1] https://libroarm.webs.uvigo.es/wp-content/uploads/2020/09/doc0673.pdf
[2] https://pdf.dzsc.com/88889/26278.pdf
[3] https://dhananjaypawar.files.wordpress.com/2016/02/arm7tdmi_programmers-model.pdf
[4] https://www.rvstcc.ac.in/assets/img/pdf/ARM7-TDMI-manual-pt1.pdf
[5] https://pdf.dzsc.com/AI-/AT91RM3400-AI-001.pdf
[6] https://documentation-service.arm.com/static/5e8e1323fd977155116a3129?token=
[7] https://documentation-service.arm.com/static/5e8e13a9fd977155116a3368?token=
[8] https://www.ele.uva.es/~jesman/BigSeti/ftp/Microcontroladores/ARM/Arm7tdmi-S%20Technical%20Reference%20Manual.pdf






