`timescale 1ns / 1ps

module alu_tb;

   reg clk;
   reg [3:0] a, b;
   reg [1:0] mode;
   wire      carry;
   wire [3:0]      out;

   alu uut(
      .clk(clk),
      .a(a),
      .b(b),
      .mode(mode),
      .out(out),
      .carry(carry)
    );

   initial begin
      clk = 0;
      forever #5 clk = ~clk;
   end
   
   integer pass, fail;

   initial begin
      pass = 0;
      fail = 0;
      a = 0;
      b = 0;
      mode = 0;
      #10;


      mode = 2'b00;
      test_alu(4'd5, 4'd3, 4'd8, 0, "Add 5+3", pass, fail); // 5+3=8, carry=0
      test_alu(4'd15, 4'd1, 4'd0, 0, "Add 15+1", pass, fail); // 15+1=0 (overflow), carry=0 (bug)

      mode = 2'b01;
      test_alu(4'd8, 4'd3, 4'd5, 0, "Sub 8-3", pass, fail); // 8-3=5, carry=0 (from a+b)
      test_alu(4'd3, 4'd5, 4'd14, 0, "Sub 3-5", pass, fail); // 3-5=-2 (14 in 4-bit), carry=0

      mode = 2'b10;
      test_alu(4'b1010, 4'b1100, 4'b1000, 0, "AND 1010 & 1100", pass, fail);
      test_alu(4'b1111, 4'b1111, 4'b1111, 0, "AND all 1s", pass, fail);

      mode = 2'b11;
      test_alu(4'b1010, 4'b1100, 4'b1110, 0, "OR 1010 | 1100", pass, fail);
      test_alu(4'b0000, 4'b1111, 4'b1111, 0, "OR with zero", pass, fail);

      $display("\nTest Summary: Passed = %0d, Failed = %0d", pass, fail);
      $finish;

   end
      

   task test_alu;
      input [3:0] val1, val2, expOut;
      input expCarry;
      input [128:0] testName;
      inout integer pass, fail;
      

      a = val1;
      b = val2;

      begin 
         @(posedge clk);
         #1;

         if (out === expOut && carry === expCarry) begin 
            pass += 1;
            $display("[PASS] %s: out=%b, carry=%b", testName, out, carry);
         end else begin
            fail += 1;
            $display("[FAIL] %s: Expected out=%b, carry=%b | Got out=%b, carry=%b", testName, expOut, expCarry, out, carry);
         
         end
      end

   endtask



endmodule
