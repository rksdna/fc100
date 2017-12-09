/*
 * Memory serial peripheral interface
 */
 
`timescale 1us / 1ns
`include "spi.v"

module spi_bench;

reg rst;

reg clk;

reg spi_clk;
reg spi_mosi;
wire spi_miso;
reg spi_ss;

wire [3:0] mem_addr;
wire [7:0] mem_din;
wire [7:0] mem_dout;
wire mem_wrt;

assign mem_din = 8'hE1;

spi dut(rst, clk, spi_clk, spi_mosi, spi_miso, spi_ss, mem_addr, mem_din, mem_dout, mem_wrt);

task shift;
    input [7:0] tmp;
begin
    repeat (8) 
    begin 
        #1;
        {spi_mosi, tmp[7:1]} = tmp[7:0];
        
        #10;
        spi_clk = 1'b1;
        
        #10;
        spi_clk = 1'b0;
   end  
end
endtask

initial 
begin
    $dumpfile("spi_bench.vcd");
    $dumpvars(0, spi_bench);
        
    rst = 1'b1;
    clk = 1'b0;
    spi_clk = 1'b0;
    spi_mosi = 1'b0;
    spi_ss = 1'b0;
    
    #10;    
    rst = 1'b0;
        
    #10;
    spi_ss = 1'b1;

    #10;
    shift(8'h0A);   
    shift(8'h00);

    #10;
    spi_ss = 1'b0;
    
    #10;
    spi_ss = 1'b1;

    #10;    
    shift(8'h8E);   
    shift(8'hA5);
    shift(8'h5A);

    #10;
    spi_ss = 1'b0;
    
    #10;
    $finish();
end

always 
begin
    #1;
    clk = !clk;
end

endmodule
