module tb;

reg clk, rst, signal;
reg [3:0] in;
wire [3:0] out;


shiftReg uut(
  .clk(clk),
  .rst(rst),
  .signal(signal),
  .in(in),
  .out(out)
);

initial begin 
  clk = 0;
  forever #5 clk = ~clk;
end

initial begin
  rst = 1;
  signal = 0;
  in = 4'b0000;
  #10; 

  rst = 0;
  #10;

  signal = 1; 
  in = 4'b1010; 
  #10;

  if (out !== 4'b1010)
    $display("Помилка: out має бути 1010, отримано %b", out);

  signal = 0;
  #10; 

  if (out !== 4'b0100) 
    $display("Помилка: out має бути 0100, отримано %b", out);

  #10;
  $finish;
end


endmodule
