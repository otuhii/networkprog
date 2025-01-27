#### Section 2: Bringup: What language is hardware coded in? -- 0.5 weeks
- Blinking an LED(Verilog, 10) -- Your first little program! Getting the simulator working. Learning Verilog.
- Building a UART(Verilog, 100) -- An intro chapter to Verilog, copy a real UART, introducing the concept of MMIO, though the serial port may be semihosting. Serial test echo program and led control.




Computer clock is just some chip that vibrates on some frequency and helps computer be synchronyzed and to access memory. I believe this is main purpose of it.


Register (reg): Used for storage elements (e.g., counters, state machines).

Wire (wire): Used for connections and combinational logic.

Parameter: Used for constants and configurability.

Clock Signal (clk): Treated as a clock because of its usage in posedge or negedge sensitivity lists. The tools infer its role based on your code.


Concepts of hdl programming: 
    -gate level - really low level, it becomes hard to write on gate level as our project gets bigger 
    -data flow level - this level is more efficient but still if you have bigger project and more complex data flow, you will have troubles to implement it, you just define whole module logic in one sentence and not in separated gates as in previous one. 
    -behavior level - using procedure block(always and ..) - the most abstract and efficient


combinational logic 
sequential logic




uart - universal asynchronous receiver transmiter

its function is to make possible serial communication, converts data from parralel(basic 00000000) form to sequential(0 -> 0 -> 0 -> 0 -> 0 -> 0 -> 0 -> 0)
uses lsb byte format, least significant 

so the most significant and interestring part is in work of trasmitter
    -first of all it sends one high signal and when it is ready to send data, sends low signal(0) and then starts to send data bits
    -after sending all bits it sends high signal

