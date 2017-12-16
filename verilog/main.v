/*
 * Reciprocal counter with vernier and serial peripheral interface
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
 *
 * rst_n - async reset
 *
 * clk - system clock
 * ref_clk - external clock 
 *
 * ch1_clk - channel 1 input
 * ch2_clk - channel 2 input
 *
 * ch1_dac - channel 1 R-2R offset DAC 
 * ch2_dac - channel 2 R-2R offset DAC 
 *
 * ch1_hpf - channel 1 DC cutoff
 * ch2_hpf - channel 1 DC cutoff
 * 
 * strt_tac_out - pulse to stop vernier
 * strt_tac_fb - feedback from stop vernier
 *
 * stop_tac_out - pulse to stop vernier
 * stop_tac_fb - feedback from stop vernier
 * 
 * spi_clk - serial clock
 * spi_mosi - serial data input
 * spi_miso - serial data output
 * spi_ss_n - serial frame sync
 * 
 * test - test LED
 */
 
`include "spi.v"
`include "counter.v"
 
module main(rst_n, clk, ref_clk, ch1_clk, ch2_clk, ch1_dac, ch2_dac, ch1_hpf, ch2_hpf, strt_tac_out, strt_tac_fb, stop_tac_out, stop_tac_fb, spi_clk, spi_mosi, spi_miso, spi_ss_n, test);

input wire rst_n;

input wire clk;
input wire ref_clk;
input wire ch1_clk;
input wire ch2_clk;

output wire [7:0] ch1_dac;
output wire [7:0] ch2_dac;

output wire ch1_hpf;
output wire ch2_hpf;

output wire strt_tac_out;
input wire strt_tac_fb;

output wire stop_tac_out;
input wire stop_tac_fb;

input wire spi_clk;
input wire spi_mosi;
output wire spi_miso;
input wire spi_ss_n;

output wire test;

wire rst;
wire spi_ss;

wire [3:0] mem_addr;
reg [7:0] mem_din;
wire [7:0] mem_dout;
wire mem_wrt;

wire strt;
wire [1:0] strt_mode;
wire [7:0] strt_dout;
wire strt_ack;

wire stop;
wire [1:0] stop_mode;
wire [7:0] stop_dout;
wire stop_ack;

wire [1:0] cnt_mode;
wire [31:0] cnt_dout;

wire tmr_mode;
wire [31:0] tmr_dout;

wire clb_zs;
wire clb_fs;

reg [7:0] dac_1_reg;
reg [7:0] dac_2_reg;
reg [6:0] mode_reg;
reg [6:0] ctrl_reg;
reg clr_reg;

assign rst = !rst_n;
assign spi_ss = !spi_ss_n;

assign ch1_dac = dac_1_reg;
assign ch2_dac = dac_2_reg;

assign strt_mode = mode_reg[1:0];
assign stop_mode = mode_reg[3:2];
assign cnt_mode = mode_reg[5:4];
assign tmr_mode = mode_reg[6];

assign strt = ctrl_reg[0];
assign stop = ctrl_reg[1];
assign clb_zs = ctrl_reg[2];
assign clb_fs = ctrl_reg[3];
assign ch1_hpf = ctrl_reg[4];
assign ch2_hpf = ctrl_reg[5];
assign test = ctrl_reg[6];

spi spi(rst, clk, spi_clk, spi_mosi, spi_miso, spi_ss, mem_addr, mem_din, mem_dout, mem_wrt);
counter counter(clr_reg || rst, clk, ref_clk, ch1_clk, ch2_clk, strt, strt_mode, strt_tac_out, strt_tac_fb, strt_dout, strt_ack, stop, stop_mode, stop_tac_out, stop_tac_fb, stop_dout, stop_ack, cnt_mode, cnt_dout, tmr_mode, tmr_dout, clb_zs, clb_fs);

always @(posedge clk or posedge rst)
begin
    if (rst)
    begin
        dac_1_reg <= 8'b0;
        dac_2_reg <= 8'b0;
        mode_reg <= 7'b0;
        ctrl_reg <= 7'b0; 
	clr_reg <= 1'b0;
    end
    else
    begin
        if (mem_wrt && mem_addr == 4'h0)
            dac_1_reg <= mem_dout;
        
        if (mem_wrt && mem_addr == 4'h1)
            dac_2_reg <= mem_dout;
            
        if (mem_wrt && mem_addr == 4'h2)
            {clr_reg, mode_reg} <= mem_dout; 
            
        if (mem_wrt && mem_addr == 4'h3)
            {clr_reg, ctrl_reg} <= mem_dout;           
    end
end

always @*
begin
    case (mem_addr)
        4'h0 : mem_din = dac_1_reg;
        4'h1 : mem_din = dac_2_reg;        
        4'h2 : mem_din = {clr_reg, mode_reg};        
        4'h3 : mem_din = {clr_reg, ctrl_reg};                                
        4'h4 : mem_din = strt_dout;
        4'h5 : mem_din = stop_dout;
        4'h6 : mem_din = {6'h0, strt_ack, stop_ack};
        4'h7 : mem_din = 8'h00;        
        4'h8 : mem_din = cnt_dout[7:0];
        4'h9 : mem_din = cnt_dout[15:8];
        4'hA : mem_din = cnt_dout[23:16];
        4'hB : mem_din = cnt_dout[31:24];
        4'hC : mem_din = tmr_dout[7:0];
        4'hD : mem_din = tmr_dout[15:8];                                                      
        4'hE : mem_din = tmr_dout[23:16];                
        4'hF : mem_din = tmr_dout[31:24];                
    endcase    
end

endmodule  
