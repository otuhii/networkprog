module uartRs(
  input wire clk,
  input wire rst,
  input wire rx,
  output reg [7:0] data,
  output reg done
);

  reg [7:0] shift_reg;
  reg [3:0] bit_count;
  reg receiving;

  always @(posedge clk or posedge rst) begin
    if (rst) begin
      data <= 0;
      done <= 0;
      receiving <= 0;

      shift_reg <= 0;
      bit_count <= 0;
    end else begin
      if (!receiving && !rx) begin 
        receiving <= 1;
        bit_count <= 0;
      end else if (receiving) begin
        if (bit_count < 8) begin
          shift_reg <= {rx, shift_reg[7:1]};
          bit_count <= bit_count + 1;
        end else begin
          data <= shift_reg;
          done <= 1;
          receiving <= 0;
        end
      end
    end
  end
endmodule
