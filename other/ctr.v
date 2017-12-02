/*
 * Reciprocal counter
 *
 * clk - reference clock
 * rst - async reset
 * ina - input A clock
 * inb - input B clock
 *
 * bis[1:0] - end mode
 *  00 - ina
 *  01 - inverted ina
 *  10 - inb
 *  10 - inverted inb
 *
 * eis[1:0] - begin mode
 *  00 - ina
 *  01 - inverted ina
 *  10 - inb
 *  10 - inverted inb
 *
 * brq - begin request
 * erq - end request
 * bac - begin acknowledge
 * eac - end acknowledge
 *
 * cta - channel 1 counter
 * ctc - channel 2 counter
 *
 * bip - run begin interpolator
 * eip - run end interpolator
 * bin - reset begin interpolator
 * ein - reset end interpolator
 *
 * ip0 - 0 interpolator calibration
 * ip1 - 1 interpolator calibration
 */

module ctr(clk, rst, ina, inb, bis, eis, brq, erq, bac, eac, cta, ctc, bip, eip, bin, ein, ip0, ip1);

parameter size = 8;

input wire clk;
input wire rst;

input wire ina;
input wire inb;

input wire [1:0] bis;
input wire [1:0] eis;

input wire brq;
input wire erq;
output reg bac;
output reg eac;

output reg [size - 1:0] cta;
output reg [size - 1:0] ctc;

output wire bip;
output wire eip;
output wire bin;
output wire ein;
input wire ip0;
input wire ip1;

wire [3:0] mux;
wire bck;
wire eck;

reg ig0;
reg ig1;

reg bg0;
reg eg0;

wire be0;
wire ee0;

reg bg1;
reg eg1;

wire be1;
wire ee1;

assign mux = {!inb, inb, !ina, ina};
assign bck = mux[bis];
assign eck = mux[eis];

assign be0 = bg0 || ig0;
assign ee0 = eg0 || ig0;

assign be1 = bg1 || ig1;
assign ee1 = eg1 || ig1;

assign bip = be0 && !bac;
assign eip = ee0 && !eac;
assign bin = !be0 && !bac;
assign ein = !ee0 && !eac;

always @(posedge bck or posedge rst)
begin
    if (rst)
    begin
        bg0 <= 1'b0;
    end
    else
    begin
        bg0 <= brq;
    end
end

always @(posedge eck or posedge rst)
begin
    if (rst)
    begin
        eg0 <= 1'b0;
    end
    else
    begin
        eg0 <= erq && bg0;
    end
end

always @(posedge ina or posedge rst)
begin
    if (rst)
    begin
        cta <= 0;
    end
    else
    begin
        if (be0 && !ee0)
        begin
            cta <= cta + 1'b1;
        end
    end
end

always @(posedge clk or posedge rst)
begin
    if (rst)
    begin
        ctc <= 0;
        bac <= 1'b0;
        eac <= 1'b0;
        bg1 <= 1'b0;
        eg1 <= 1'b0;
        ig0 <= 1'b0;
        ig1 <= 1'b0;
    end
    else
    begin
        if (be1 && !ee1)
        begin
            ctc <= ctc + 1'b1;
        end
        bg1 <= be0;
        eg1 <= ee0;
        bac <= be1;
        eac <= ee1;
        ig0 <= ip1 || ip0;
        ig1 <= ip0;
    end
end

endmodule
