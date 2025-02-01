module tb_uart;


  
  reg clk;
  reg rst;
  reg [7:0] tx_data;
  reg start;

  wire tx;
  wire [7:0] rx_data;
  wire rx_done;

  uartTx uutTx(
    .clk(clk),
    .rst(rst),
    .data(tx_data),
    .start(start),
    .tx(tx)
  );


  uartRs uutRs(
    .clk(clk),
    .rst(rst),
    .rx(tx),
    .data(rx_data),
    .done(rx_done)
  );

  initial begin 
    clk = 0;
    forever #10 clk = ~clk;
  end


  initial begin 
    rst = 1;
    tx_data = 0;
    #100;

    rst = 0;
    #100;

    tx_data = 8'hA5;
    start = 1;
    #20;
    start = 0;

    wait(rx_done);
    $display("Receiver data: %h", rx_data);


    #100;
    $finish;

  end


  //gtkwave "simulation"
  initial begin
    $dumpfile("waveform.vcd");
    $dumpvars(0, tb_uart);
  end

endmodule




