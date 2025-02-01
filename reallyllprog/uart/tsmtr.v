module uartTx (
  input wire start,
  input wire rst,
  input wire clk,
  input wire [7:0] data,
  output reg tx
);
  reg [3:0] bit_count;
  reg [7:0] shift_reg;
  reg transmitting;

  always @(posedge clk or posedge rst) begin
    if (rst) begin
      //just idle mode
      tx <=1;
      bit_count <=0;
      shift_reg <=0;
      transmitting <= 0;
    end else begin 
      if (start && !transmitting) begin
      //indicating that we want to start transmitting and moving data into shift buffer
        transmitting <= 1;
        shift_reg <= data;
        bit_count <= 0;
        tx <= 0;
      end else if (transmitting) begin
        //actual transmit of data
        if (bit_count < 8) begin 
          tx <= shift_reg[0];
          shift_reg <= shift_reg >> 1;
          bit_count <= bit_count + 1;
        end else begin 
          tx <= 1;
          transmitting <= 0; 
        end
      end
    end
  end

endmodule
