`timescale 1us / 1ns
`include "spi.v"

module spi_test;
	
    reg nrst;
    reg clk; 

    reg sck;
    reg mosi;
    wire miso;
    reg nss;

    wire [7:0] di;
    wire [7:0] do;
    wire we;
    wire re;

    reg [7:0] sh;

	spi dut (nrst, clk, sck, mosi, miso, nss, di, do, we, re);

    assign di = re ? 8'hA1 : 8'bZ;

	initial 
    begin
		$dumpfile("spi_bench.vcd");
		$dumpvars(0, spi_test);
        
        nrst = 0;
        clk = 1;
        sck = 0;
        mosi = 0;
        nss = 1;
        sh = 8'h85;

		#10 nrst = 1;
        #10 nss = 0;

        repeat (8) 
        begin 
            mosi = sh[7];
            sh = sh << 1;
            #5 sck = 1;
            #5 sck = 0;
            #2; 
         end 

        nss = 1;
		#10 $finish();
    end

	always 
		#1 clk = !clk;

endmodule
