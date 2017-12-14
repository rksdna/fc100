/*
 * Reciprocal counter with vernier (test bench)
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
`include "counter.v"

module counter_bench;

reg rst;

reg clk;
reg ref_clk;

reg ch1_clk;
reg ch2_clk;

reg strt;
reg [1:0] strt_mode;
wire strt_tac_out;
reg strt_tac_fb;
wire [7:0] strt_dout;
wire strt_ack;

reg stop;
reg [1:0] stop_mode;
wire stop_tac_out;
reg stop_tac_fb;
wire [7:0] stop_dout;
wire stop_ack;

reg [1:0] cnt_mode;
wire [31:0] cnt_dout;

reg tmr_mode;
wire [31:0] tmr_dout;

reg clb_zs;
reg clb_fs;

counter dut(rst, clk, ref_clk, ch1_clk, ch2_clk, strt, strt_mode, strt_tac_out, strt_tac_fb, strt_dout, strt_ack, stop, stop_mode, stop_tac_out, stop_tac_fb, stop_dout, stop_ack, cnt_mode, cnt_dout, tmr_mode, tmr_dout, clb_zs, clb_fs);

initial 
begin
    $dumpfile("counter_bench.vcd");
    $dumpvars(0, counter_bench);
		
    rst = 1'b1;
    
    clk = 1'b0;
    ref_clk = 1'b0;
    
    ch1_clk = 1'b0;
    ch2_clk = 1'b0;
    
    strt = 1'b0;
    strt_mode = 2'b0;
    strt_tac_fb = 1'b0;
  
    stop = 1'b0;
    stop_mode = 2'b0;
    stop_tac_fb = 1'b0;
    
    cnt_mode = 2'b0;
    tmr_mode = 1'b0;    
    
    clb_zs = 1'b0;
    clb_fs = 1'b0;   
    
    /* zero scale */
    #10;    
    rst = 1'b0;
    
    #14
    clb_zs = 1'b1;
    
    #100;
    rst = 1'b1;
    clb_zs = 1'b0; 
  
    /* full scale */
    #10;    
    rst = 1'b0;
    
    #14
    clb_fs = 1'b1;
    
    #100;
    rst = 1'b1;
    clb_fs = 1'b0;        
  
    /* frequency */
    #10;    
    rst = 1'b0;

    #10;
    strt = 1'b1;

    #100;
    stop = 1'b1;
    
    #100;
    rst = 1'b1;
    strt = 1'b0;
    stop = 1'b0;
    
    /* time high */
    #10;    
    rst = 1'b0;
    stop_mode = 2'b1;

    #10;
    strt = 1'b1;
    stop = 1'b1;
    
    #100;
    rst = 1'b1;
    stop_mode = 2'b0;
    strt = 1'b0;
    stop = 1'b0;	
    
    #10;
    $finish();
end

always 
begin
    #16;
    ch1_clk = !ch1_clk;
end

always 
begin
    #5;
    ch2_clk = !ch2_clk;
end

always 
begin
    #4;
    clk = !clk;
end

always 
begin
    #3;
    ref_clk = !ref_clk;
end

always @(posedge strt_tac_out)
begin
    if (!rst)
    begin
        #3;
        strt_tac_fb = 1'b1; 
    
        #47;
        strt_tac_fb = 1'b0; 
    end
end

always @(posedge stop_tac_out)
begin
    if (!rst)
    begin
        #3;
        stop_tac_fb = 1'b1; 
    
        #47;
        stop_tac_fb = 1'b0; 
    end        
end

endmodule
