/*
 * Reciprocal counter
 *
 * rst - async reset
 *
 * clk - internal reference clock
 * rfc - external reference clock
 *
 * ina - channel A input
 * inb - channel B input
 *
 * bis[1:0] - begin trigger selection
 *  00 - ina
 *  01 - inverted ina
 *  10 - inb
 *  10 - inverted inb
 *
 * eis[1:0] - end trigger selection
 *  00 - ina
 *  01 - inverted ina
 *  10 - inb
 *  10 - inverted inb
 *
 * brq - begin trigger request
 * erq - end trigger request
 * bac - begin trigger acknowledge
 * eac - end trigger acknowledge
 *
 * xis[1:0] - main input select
 *  00 - ina
 *  01 - inverted ina
 *  10 - inb
 *  10 - inverted inb
 *
 * cnx - input counter
 * 
 * ris - reference input select
 *  0 - clk
 *  1 - rfc
 *
 * cnr - reference counter
 *
 * bip - begin interpolator enable
 * eip - end interpolator enable
 * bin - begin interpolator clear
 * ein - end interpolator clear
 *
 * ip0 - 0 interpolator calibration
 * ip1 - 1 interpolator calibration
 */

module ctr(rst, clk, rfc, ina, inb, bis, eis, brq, erq, bac, eac, xis, cnx, ris, cnr, bip, eip, bin, ein, ip0, ip1);

parameter size = 8;

input wire rst;

input wire clk;
input wire rfc;

input wire ina;
input wire inb;

input wire [1:0] bis;
input wire [1:0] eis;

input wire brq;
input wire erq;
output reg bac;
output reg eac;

input wire [1:0] xis;
output reg [size - 1:0] cnx;

input wire ris;
output reg [size - 1:0] cnr;

output wire bip;
output wire eip;
output wire bin;
output wire ein;
input wire ip0;
input wire ip1;

wire [3:0] mux;
wire bck;
wire eck;
wire xck;
wire rck;

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
assign xck = mux[xis];
assign rck = ris ? rfc : clk;

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

always @(posedge xck or posedge rst)
begin
    if (rst)
    begin
        cnx <= 0;
    end
    else
    begin
        if (be0 && !ee0)
        begin
            cnx <= cnx + 1'b1;
        end
    end
end

always @(posedge rck or posedge rst)
begin
    if (rst)
    begin
        cnr <= 0;
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
            cnr <= cnr + 1'b1;
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
