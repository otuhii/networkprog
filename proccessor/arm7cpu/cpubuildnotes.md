some my thoughts notes and questions/answers will be here along my cpu building way


register - combination of small latches that are combination of transistor assembled in the way to store or read some signal

ram - stored as list of matrices because it is more efficient to address

sram - fast but expensive 
dram - slower but less expensive(has smaller number of transistors, because it uses not those logic gates but specific transistor mosfet and capacitor)

--- whole series of this videos is amazing 
https://www.youtube.com/watch?v=GYlNoAMBY6o&list=PL9vTTBa7QaQOoMfpP3ztvgyQkPWDPfJez&index=4
---
  
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


