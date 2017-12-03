/*
 * Serial peripheral interface
 */
 
`timescale 1us / 1ns
`include "spi.v"

module spi_bench;

reg rst;

reg clk;

reg sck;
reg sdi;
wire sdo;
reg scs;

wire [7:0] pdi;
wire [7:0] pdo;

defparam dut.size = 8;

spi dut(rst, clk, sck, sdi, sdo, scs, pdi, pdo);

assign pdi = 8'h81;

task shift;
    input [7:0] tmp;
begin
    scs = 1'b1;
    repeat (8) 
    begin 
        #1;
        {sdi, tmp[7:1]} = tmp[7:0];
        
        #10;
        sck = 1'b1;
        
        #10;
        sck = 1'b0;
   end  
   scs = 1'b0; 
end
endtask

initial 
begin
    $dumpfile("spi_bench.vcd");
    $dumpvars(0, spi_bench);
        
    rst = 1'b1;
    clk = 1'b0;
    sck = 1'b0;
    sdi = 1'b0;
    scs = 1'b0;
    
    #10;    
    rst = 1'b0;
        
    #10;
    shift(8'hA5);
    
    #10;    
    shift(8'h5A);
    
    #10;
    $finish();
end

always 
begin
    #1;
    clk = !clk;
end

endmodule
