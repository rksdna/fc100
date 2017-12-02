/*
 * Reciprocal counter
 */

`timescale 1us / 1ns
`include "ctr.v"

module ctr_bench;
	
    reg clk;
    reg rst;

    reg ina;
    reg inb;

    reg [1:0] bis;
    reg [1:0] eis;

    reg brq;
    reg erq;
    wire bac;
    wire eac;

    wire [31:0] cta;
    wire [31:0] ctc;

    wire bip;
    wire eip;
    wire bin;
    wire ein;
    reg ip0;
    reg ip1;

    ctr dut(clk, rst, ina, inb, bis, eis, brq, erq, bac, eac, cta, ctc, bip, eip, bin, ein, ip0, ip1);

initial 
begin
    $dumpfile("ctr_bench.vcd");
    $dumpvars(0, ctr_bench);
		
    rst = 1'b1;
    clk = 1'b0;
    
    ina = 1'b0;
    inb = 1'b0;
    
    bis = 2'b00;
    eis = 2'b00;
    
    brq = 1'b0;
    erq = 1'b0;
    
    ip0 = 1'b0;    
    ip1 = 1'b0;    

    /* calibration 0 */
    #10;    
    rst = 1'b0;
    
    #10;
    ip0 = 1'b1;
    
    #50;
    ip0 = 1'b0;
    rst = 1'b1;
         
    /* calibration 1 */    

    #10;    
    rst = 1'b0;
    
    #10;
    ip1 = 1'b1;
    
    #50;
    ip1 = 1'b0;
    rst = 1'b1;      
    
    /* time low */
    #10; 
    rst = 1'b0;
    bis = 2'b01;
    eis = 2'b00;
    
    #10;
    brq = 1'b1;
    erq = 1'b1;
    
    #50;
    rst = 1'b1;
    brq = 1'b0;
    erq = 1'b0;
    
    /* frequency */
    #10;    
    rst = 1'b0;
    bis = 2'b00;
    eis = 2'b00;

    #10;
    brq = 1'b1;

    #100;
    erq = 1'b1;
    
    #30;
    rst = 1'b1;
    brq = 1'b0;
    erq = 1'b0;	
    
    #10;
    $finish();
end

always 
begin
    #7;
    ina = !ina;
end

always 
begin
    #5;
    inb = !inb;
end

always 
begin
    #4;
    clk = !clk;
end

endmodule


