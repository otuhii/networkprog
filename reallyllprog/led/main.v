module blink_led (
    input wire clk,      
    output reg led        
);

    reg [31:0] counter = 0;  // 32-bit counter
 
    always @(posedge clk) begin
        if (counter == 50_000_000) begin
            counter <= 0;   // Reset the counter
            led <= ~led;    
        end else begin
            counter <= counter + 1;  // Increment the counter
        end
    end

endmodule
