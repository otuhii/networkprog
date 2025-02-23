some my thoughts notes and questions/answers will be here along my cpu building way



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

#### **3. Verilog for Hardware Design**
   - **Basics**:
     - Syntax for modules, wires, registers, and always blocks.
     - Finite State Machines (FSMs) for pipeline control.
   - **Tools**:
     - **Verilator**: Simulate your CPU design.
     - **GTKWave**: Visualize waveforms for debugging.

#### **4. Memory Subsystem**
   - **Harvard vs. Von Neumann Architecture**:
     - ARM7 uses Von Neumann (shared instruction/data memory).
   - **Memory-Mapped I/O**:
     - How peripherals (e.g., UART) interact with the CPU.

---


- **Verilator**: Simulate your CPU and catch logic errors early.
- **GDB/OpenOCD**: If testing on FPGA later.
- **Waveform viewers (GTKWave)**: Debug pipeline stages.

### **Common Pitfalls to Avoid**
1. **Pipeline Stalls**: Ignoring data hazards (e.g., using a register before it’s written).
2. **Endianness**: ARM is little-endian; ensure memory reads/writes match.
3. **Branch Handling**: Forgetting to update PC for `b` or `bl` instructions.


