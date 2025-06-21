//sequential logic
//interesting thing that using for example in 22 line expression like out <=
//data is slower and would give incorrect values on tbs, same as 26 line and
//out <= data expression would give incorrect values 

module shiftReg(
  input           clk,
  input           rst,
  input           signal,
  input     [3:0] in,
  output reg[3:0] out
);


reg [3:0] data;

  
always @(negedge clk or posedge rst) begin 
  
  if (rst) begin 
    data <= 4'b0;
    out <= data;
  end else begin
    if (signal == 1) begin
      data <= in;
      out <= in;
    end  
    if (signal == 0) begin 
      data <= data << 1;
      out <= data << 1;
    end
  
  end 

end

endmodule
