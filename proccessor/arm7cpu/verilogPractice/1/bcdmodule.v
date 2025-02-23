//just basic double dabble for hundreds, tens and units
//easy shit 

module bcd(
  input             clk,
  input             rst,
  input [7:0]       bin,
  output reg [3:0]  hunds,
  output reg [3:0]  tens, 
  output reg [3:0]  units,
  output reg complete
);


reg [19:0] shiftReg;

parameter IDLE = 0, SHIFT = 1, ADD = 2, DONE = 3;

reg [1:0] st;
reg [3:0] counter;


always @(posedge clk or posedge rst) begin 
  if (rst) begin 
    hunds <= 4'b0;
    tens <= 4'b0;
    units <= 4'b0;
    shiftReg <= 20'b0;
    counter <= 4'b0;
    st <= IDLE;
  end else begin 
    case (st)
      IDLE: begin 
        shiftReg <= {12'b0, bin};
        st <= SHIFT;
        counter <= 0;
      end 
      
      SHIFT : begin 
        shiftReg <= shiftReg << 1;
        st <= ADD; 
      end 
    
      ADD: begin 
        if (shiftReg[19:16] >= 5)
          shiftReg[19:16] <= shiftReg[19:16] + 3;
        if (shiftReg[15:12] >= 5)
          shiftReg[15:12] <= shiftReg[15:12] + 3;
        if (shiftReg[11:8] >= 5)
          shiftReg[11:8] <= shiftReg[11:8] + 3;
        

        if (counter >= 7) 
          st <= DONE;
        else begin 
          counter <= counter + 1;
          st <= SHIFT;
        end
      end 

      DONE: begin 
        hunds <= shiftReg[19:16];
        tens <= shiftReg[15:12];
        units <= shiftReg[11:8];
        complete <= 1;
        st <= IDLE;
      end

    endcase
  end
end





endmodule


