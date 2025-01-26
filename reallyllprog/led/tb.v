module tb;

    reg clk;
    wire led;


    // Instantiate the Unit Under Test (UUT)
    blink_led uut (
        .clk(clk),
        .led(led)
    );

    // Clock generation
    initial begin
        clk = 0;
        forever #10 clk = ~clk;  // Toggle clock every 10 time units
    end

    initial begin 

      $dumpfile("waveform.vcd");
      $dumpvars(0, tb);


      #10000000;
      $finish;
    end

endmodule
