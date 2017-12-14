/*
 * Memory serial peripheral interface (test bench)
 * Copyright (c) 2017 rksdna, murych
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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
