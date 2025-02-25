//add - 00, sub - 01, and - 10, or - 11
module alu (
            input        clk,
            input [3:0]  a,
            input [3:0]  b,
            input [1:0]  mode,
            output [3:0] out,
            output       carry
);

   wire [4:0] tmp;
   reg  [3:0] result;

   assign out = result;
   assign tmp = {1'b0, a} + {1'b0, b};
   
   assign carry = (mode == SUM) ? tmp[4] : 2'b00;

   parameter SUM = 2'b00, SUB = 2'b01, AND = 2'b10, OR = 2'b11;


   always @(posedge clk) begin
       case (mode)
           SUM:  
              result <= a + b;
           SUB: begin
              result <= a - b;
           end
           AND: begin 
              result <= a & b;
           end
           OR: begin  
              result <= a | b;
           end
       endcase
   end


endmodule
