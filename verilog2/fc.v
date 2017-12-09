/*
 * Frequency counter
 *
 * rst_n - async reset
 *
 * clk - system clock
 *
 * spi_clk - serial clock
 * spi_mosi - serial data input
 * spi_miso - serial data output
 * spi_ss_n - serial frame sync
 * 
 */

`include "spi.v"
 
module fc(rst_n, clk, spi_clk, spi_mosi, spi_miso, spi_ss_n, test);

input wire rst_n;

input wire clk;

input wire spi_clk;
input wire spi_mosi;
output wire spi_miso;
input wire spi_ss_n;

output wire test;

wire [3:0] mem_addr;
wire [7:0] mem_din;
wire [7:0] mem_dout;
wire mem_wrt;

reg [7:0] r0;
reg [7:0] r1;

wire [7:0] mem_mux[1:0];

assign mem_mux[0] = r0;
assign mem_mux[1] = r1;

assign test = r0[7];

assign mem_din = mem_mux[mem_addr];

spi spi(!rst_n, clk, spi_clk, spi_mosi, spi_miso, !spi_ss_n, mem_addr, mem_din, mem_dout, mem_wrt);

always @(posedge clk or negedge rst_n)
begin
    if (!rst_n)
    begin
        r0 <= 8'b0;
        r1 <= 8'b0;
    end
    else
    begin
        if (mem_wrt && mem_addr == 0)
            r0 <= mem_dout;
            
        if (mem_wrt && mem_addr == 1)
            r1 <= mem_dout;
    end
end

endmodule  
