/*
 * Reciprocal counter with vernier
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
 * rst - async reset
 *
 * clk - internal clock
 * ref_clk - external clock 
 *
 * ch1_clk - channel 1 input
 * ch2_clk - channel 2 input
 *
 * strt - start request
 * strt_mode - start clock selection
 * - 00 - ch1_clk
 * - 01 - ch1_clk inverted
 * - 10 - ch2_clk
 * - 11 - ch2_clk inverted
 * strt_tac_out - pulse to start vernier
 * strt_tac_fb - feedback from start vernier
 * strt_dout - value of start vernier
 * strt_ack - start acknowledge
 * 
 * stop - stop request
 * stop_mode - stop clock selection
 * - 00 - ch1_clk
 * - 01 - ch1_clk inverted
 * - 10 - ch2_clk
 * - 11 - ch2_clk inverted
 * stop_tac_out - pulse to stop vernier
 * stop_tac_fb - feedback from stop vernier
 * stop_dout - value of stop vernier
 * stop_ack - stop acknowledge
 *
 * cnt_mode - counter clock selection
 * - 00 - ch1_clk
 * - 01 - ch1_clk inverted
 * - 10 - ch2_clk
 * - 11 - ch2_clk inverted 
 * cnt_dout - counter value
 *
 * tmr_mode - timer clock selection
 * - 0 - clk
 * - 1 - ref_clk
 * tmr_dout - counter value
 *
 * clb_zs - start and stop vernier zero scale calibration 
 * clb_fs - start and stop vernier full scale calibration
 */

module counter(rst, clk, ref_clk, ch1_clk, ch2_clk, strt, strt_mode, strt_tac_out, strt_tac_fb, strt_dout, strt_ack, stop, stop_mode, stop_tac_out, stop_tac_fb, stop_dout, stop_ack, cnt_mode, cnt_dout, tmr_mode, tmr_dout, clb_zs, clb_fs);

input wire rst;

input wire clk;
input wire ref_clk;

input wire ch1_clk;
input wire ch2_clk;

input wire strt;
input wire [1:0] strt_mode;
output wire strt_tac_out;
input wire strt_tac_fb;
output wire [7:0] strt_dout;
output wire strt_ack;

input wire stop;
input wire [1:0] stop_mode;
output wire stop_tac_out;
input wire stop_tac_fb;
output wire [7:0] stop_dout;
output wire stop_ack;

input wire [1:0] cnt_mode;
output wire [31:0] cnt_dout;

input wire tmr_mode;
output wire [31:0] tmr_dout;

input wire clb_zs;
input wire clb_fs;

wire [3:0] cnt_mux;
wire [1:0] tmr_mux;

wire strt_clk;
wire stop_clk;
wire cnt_clk;
wire tmr_clk;

reg strt_reg;
reg stop_reg;

wire [1:0] strt_gate;
reg [1:0] strt_gate_reg;
reg [7:0] strt_tmr;

wire [1:0] stop_gate;
reg [1:0] stop_gate_reg;
reg [7:0] stop_tmr;

reg [1:0] clb_reg;

reg [31:0] cnt;
reg [31:0] tmr;

assign cnt_mux = {!ch2_clk, ch2_clk, !ch1_clk, ch1_clk};
assign tmr_mux = {ref_clk, clk};

assign strt_clk = cnt_mux[strt_mode];
assign stop_clk = cnt_mux[stop_mode];
assign cnt_clk = cnt_mux[cnt_mode];
assign tmr_clk = tmr_mux[tmr_mode];

assign strt_gate[0] = strt_reg || clb_reg[0];
assign strt_gate[1] = strt_gate_reg[0] || clb_reg[1];

assign stop_gate[0] = stop_reg || clb_reg[0];
assign stop_gate[1] = stop_gate_reg[0] || clb_reg[1];

assign strt_tac_out = strt_gate[0] && !strt_gate_reg[1];
assign stop_tac_out = stop_gate[0] && !stop_gate_reg[1];

assign strt_ack = strt_gate_reg[1] && !strt_tac_fb;
assign stop_ack = stop_gate_reg[1] && !stop_tac_fb;

assign strt_dout = strt_tmr;
assign stop_dout = stop_tmr;

assign cnt_dout = cnt;
assign tmr_dout = tmr;

always @(posedge strt_clk or posedge rst)
begin
    if (rst)
        strt_reg <= 1'b0;
    else
        strt_reg <= strt;
end

always @(posedge stop_clk or posedge rst)
begin
    if (rst)
        stop_reg <= 1'b0;
    else
        stop_reg <= stop && strt_reg;
end

always @(posedge cnt_clk or posedge rst)
begin
    if (rst)
    begin
        cnt <= 31'b0;
    end
    else
    begin
        if (strt_gate[0] && !stop_gate[0])
            cnt <= cnt + 1'b1;
    end
end

always @(posedge tmr_clk or posedge rst)
begin
    if (rst)
    begin
        strt_gate_reg <= 2'b0;
        strt_tmr <= 8'b0;
        
        stop_gate_reg <= 2'b0;
        stop_tmr <= 8'b0;
                
        clb_reg <= 2'b0;
                
        tmr <= 31'b0;
    end
    else
    begin
        strt_gate_reg <= strt_gate;
        if (strt_gate_reg[1] && strt_tac_fb)
            strt_tmr <= strt_tmr + 1'b1;
            
        stop_gate_reg <= stop_gate;
        if (stop_gate_reg[1] && stop_tac_fb)
            stop_tmr <= stop_tmr + 1'b1;
            
        clb_reg <= {clb_zs, clb_zs || clb_fs};
    
        if (strt_gate[1] && !stop_gate[1])
            tmr <= tmr + 1'b1;
    end
end

endmodule

