/*
 * Reciprocal counter
 */

`timescale 1us / 1ns
`include "ctr.v"

module ctr_bench;

    reg rst;
	
    reg clk;
    reg rfc;

    reg ina;
    reg inb;

    reg [1:0] bis;
    reg [1:0] eis;

    reg brq;
    reg erq;
    wire bac;
    wire eac;

    reg [1:0] xis;
    wire [7:0] cnx;
    
    reg ris;
    wire [7:0] cnr;

    wire bip;
    wire eip;
    wire bin;
    wire ein;
    reg ip0;
    reg ip1;
    
    defparam dut.size = 8;

    ctr dut(rst, clk, rfc, ina, inb, bis, eis, brq, erq, bac, eac, xis, cnx, ris, cnr, bip, eip, bin, ein, ip0, ip1);

initial 
begin
    $dumpfile("ctr_bench.vcd");
    $dumpvars(0, ctr_bench);
		
    rst = 1'b1;
    
    clk = 1'b0;
    rfc = 1'b0;
    
    ina = 1'b0;
    inb = 1'b0;
    
    bis = 2'b00;
    eis = 2'b00;
    
    xis = 2'b00;
    ris = 1'b0;
    
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

always 
begin
    #3;
    rfc = !rfc;
end

endmodule


