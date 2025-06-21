module bcd_tb;

reg clk, rst;
reg [7:0] number;
wire [3:0] hunds, tens, units;
wire completeness;


bcd uut (
  .clk(clk),
  .rst(rst),
  .bin(number),
  .hunds(hunds),
  .tens(tens),
  .units(units),
  .complete(completeness)
);

initial begin 
  clk = 0;
  forever #5 clk = ~clk;

end


//test
initial begin 
  rst = 1;
  #10 rst = 0;

  //99 
  number = 8'b01100011;
  wait(completeness);
  #100;

  $finish;

end


always @(posedge completeness) begin
  $display("Binary: %b | Hundreds: %d, Tens: %d, Units: %d",
           number, hunds, tens, units);
end

endmodule
