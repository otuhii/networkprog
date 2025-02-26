dep seek xd
Here’s a list of small to medium-sized Verilog projects that are fundamental but not too easy or too hard. These projects will help you practice Verilog and prepare you for implementing CPU components like registers, ALUs, and more. Each project can be completed in one evening and will reinforce your understanding of Verilog concepts.

---

### **1. Basic Combinational Logic Circuits**
- **Project**: Implement a 4-bit binary to BCD (Binary-Coded Decimal) converter.
  - Input: 4-bit binary number.
  - Output: Two 4-bit BCD digits (tens and units).
  - This will help you practice combinational logic and case statements.

DONE!!


---


### **2. Sequential Logic: Registers and Counters**
- **Project**: Design a 4-bit shift register with parallel load.
  - Inputs: Clock, reset, load signal, 4-bit parallel input.
  - Output: 4-bit serial output.
  - Functionality: On a clock edge, the register should shift its contents or load new data based on the load signal.


DONE !!
---


### **4. Arithmetic Logic Unit (ALU)**
- **Project**: Design a simple 4-bit ALU.
  - Inputs: Two 4-bit operands (A and B), 2-bit operation code (e.g., 00 = add, 01 = subtract, 10 = AND, 11 = OR).
  - Output: 4-bit result.
  - Optional: Add a carry-out flag for addition/subtraction.


DONE!!
---

### **5. Memory Unit**
- **Project**: Implement a small RAM module.
  - Inputs: Address (e.g., 4 bits), data in (e.g., 8 bits), write enable, clock.
  - Output: Data out (8 bits).
  - Functionality: Write data to a specific address or read data from an address.
---



i dont care(( like i gonna start writing cpu and actually see how all this connects and so. I grasped basics of verilog and everything else would find out in process of building




### **6. Multiplexers and Demultiplexers**
- **Project**: Design a 4-to-1 multiplexer and a 1-to-4 demultiplexer.
  - Multiplexer: 4 inputs, 2-bit select line, 1 output.
  - Demultiplexer: 1 input, 2-bit select line, 4 outputs.

---

### **7. Clock Divider**
- **Project**: Create a clock divider module.
  - Input: Clock signal.
  - Output: Slower clock signal (e.g., divide by 2, 4, or 8).
  - This will help you understand how to manipulate clock signals.

---

### **8. Seven-Segment Display Decoder**
- **Project**: Design a 4-bit binary to seven-segment display decoder.
  - Input: 4-bit binary number.
  - Output: 7-bit signal to drive a seven-segment display.
  - This will help you practice combinational logic and case statements.

---

### **9. Debouncing Circuit**
- **Project**: Implement a debouncing circuit for a button input.
  - Input: Noisy button signal.
  - Output: Clean, debounced signal.
  - This will help you understand how to handle real-world inputs.

---

### **10. Simple CPU Component: Program Counter**
- **Project**: Design a 4-bit program counter (PC) with reset and increment functionality.
  - Inputs: Clock, reset, increment signal.
  - Output: 4-bit PC value.
  - Functionality: On a clock edge, the PC should reset to 0 or increment by 1.

---

### **11. Simple CPU Component: Instruction Decoder**
- **Project**: Create a basic instruction decoder for a hypothetical 8-bit CPU.
  - Input: 8-bit instruction.
  - Output: Control signals (e.g., ALU operation, register write enable, etc.).
  - This will help you understand how CPUs decode instructions.

---

### **12. Pipeline Register**
- **Project**: Implement a simple pipeline register.
  - Inputs: Clock, reset, data in (e.g., 8 bits).
  - Output: Data out (8 bits).
  - Functionality: On a clock edge, the register should store the input data and output it on the next cycle.

---

### **13. Signed Number Adder/Subtractor**
- **Project**: Design a 4-bit signed adder/subtractor.
  - Inputs: Two 4-bit signed numbers, add/subtract control signal.
  - Output: 4-bit result, overflow flag.
  - This will help you understand how signed numbers are handled in hardware.

---

### **14. Priority Encoder**
- **Project**: Implement an 8-to-3 priority encoder.
  - Input: 8-bit input.
  - Output: 3-bit binary code representing the highest priority active input.
  - This will help you practice conditional logic.

---

### **15. Simple Cache Controller**
- **Project**: Design a basic cache controller for a direct-mapped cache.
  - Inputs: Address, data, read/write signal.
  - Output: Data out, hit/miss signal.
  - This will introduce you to memory hierarchy concepts.

---

### **16. FIFO Buffer**
- **Project**: Implement a small FIFO (First-In-First-Out) buffer.
  - Inputs: Clock, reset, data in, write enable, read enable.
  - Outputs: Data out, full flag, empty flag.
  - This will help you understand how to manage data flow.

---

### **17. Simple Interrupt Controller**
- **Project**: Design a basic interrupt controller.
  - Inputs: Multiple interrupt request signals, mask register.
  - Output: Interrupt signal to CPU.
  - This will help you understand how interrupts are handled in hardware.

---

### **18. CRC Generator**
- **Project**: Implement a CRC (Cyclic Redundancy Check) generator for a small data packet.
  - Input: Data stream.
  - Output: CRC code.
  - This will help you practice sequential logic and bit manipulation.

---

### **19. Simple Cache Memory**
- **Project**: Design a small direct-mapped cache memory.
  - Inputs: Address, data, read/write signal.
  - Output: Data out, hit/miss signal.
  - This will help you understand how caches work.

---

### **20. Basic CPU Datapath**
- **Project**: Implement a simple datapath for a hypothetical CPU.
  - Components: ALU, registers, multiplexers, program counter.
  - Functionality: Perform basic operations like add, subtract, and load/store.

---


# Пояснення часової шкали Verilog та виконання модулів

Я поясню українською концепцію часової шкали у Verilog та як виконуються модулі в тестовому середовищі.

## Основна концепція виконання модулів у Verilog

У Verilog модулі не "викликаються" як функції в інших мовах програмування. Усі модулі існують паралельно і постійно активні. Коли ви створюєте екземпляр модуля (як `bcd uut` у вашому коді), модуль автоматично стає частиною схеми моделювання.

## Як це працює у вашому тестовому середовищі

1. **Інстанціювання модуля**: Ви створили екземпляр модуля `bcd` з назвою `uut` та підключили його порти до відповідних сигналів тестового середовища. Цей екземпляр активний протягом усього моделювання.

2. **Часова шкала і події**: Verilog використовує дискретну часову шкалу для моделювання. У вашому коді є кілька часових конструкцій:
   - `forever #5 clk = ~clk;` - генерує тактові імпульси кожні 5 одиниць часу
   - `#10 rst = 0;` - затримка скидання після 10 одиниць часу
   - `wait(completeness);` - очікування сигналу завершення перед продовженням

3. **Паралельне виконання**: Усі блоки `initial` та `always` у вашому коді виконуються паралельно:
   - Перший блок `initial` генерує тактові імпульси
   - Другий блок `initial` керує сигналами `rst` та `number`
   - Блок `always @(posedge completeness)` виконується щоразу, коли `completeness` змінюється з 0 на 1

## Як модуль `bcd` знає, коли виконуватися

Модуль `bcd` реагує на зміни вхідних сигналів (`clk`, `rst`, `bin`). Всередині модуля `bcd` є процеси (блоки `always` або призначення безперервних сигналів), які активуються, коли змінюються їхні тригерні сигнали.

Наприклад, якщо в модулі `bcd` є блок `always @(posedge clk)`, він виконується кожен раз, коли сигнал `clk` змінюється з 0 на 1. Оскільки ваше тестове середовище генерує тактові імпульси кожні 5 одиниць часу, внутрішні процеси модуля `bcd` будуть виконуватися синхронно з цим сигналом.

## Порядок моделювання

1. Симулятор ініціалізує всі сигнали
2. Виконуються всі блоки `initial`
3. Симулятор просувається по часовій шкалі
4. При зміні сигналів виконуються відповідні блоки `always`
5. Процес продовжується до виклику `$finish`

На відміну від звичайних мов програмування, де функції викликаються послідовно, у Verilog модулі працюють паралельно, реагуючи на події, які відбуваються в системі. Це відображає природу апаратних систем, де всі компоненти функціонують одночасно.





timescale - перше число визначає загалом одиниці виміру таких штук як #10 або інші часові дефініції, а друге число показує яка є найменша одиниця яку можна розрізняти як окрему одиницю часу(для мене воно схоже на якісь числа з комою, там теж тіпа дабл пресішин і тд)
