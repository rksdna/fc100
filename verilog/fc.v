/* 
 * Frequency meter    
 *
 * n_rst - async reset
 *
 * clk - internal reference clock
 * rfc - external reference clock
 *
 * ina - channel A input
 * inb - channel B input
 *
 * sck - serial clock
 * sdi - serial data input
 * sdo - serial data output
 * n_scs - serial select
 *
 * bip - begin interpolator enable
 * eip - end interpolator enable
 * bin - begin interpolator clear
 * ein - end interpolator clear
 *
 * tst - test led output
 */ 
 
`include "spi.v"
`include "ctr.v"

module fc(n_rst, clk, rfc, ina, inb, sck, sdi, sdo, n_scs, bip, eip, bin, ein, tst);

parameter cnr_size = 32;
parameter spi_size = 2 * cnr_size + 8;

input wire n_rst;

input wire clk;
input wire rfc;

input wire ina;
input wire inb;

input wire sck;
input wire sdi;
output wire sdo;
input wire n_scs;

output wire bip;
output wire eip;

output wire bin;
output wire ein;

output wire tst;

wire [1:0] bis;
wire [1:0] eis;
wire [1:0] xis;
wire ris;
wire brq;
wire erq;
wire ip0;
wire ip1;
wire bac;
wire eac;

wire [cnr_size - 1:0] cnx;
wire [cnr_size - 1:0] cnr;

wire [spi_size - 1:0] pdi;
wire [spi_size - 1:0] pdo;

defparam ctr.size = cnr_size;
defparam spi.size = spi_size;

assign pdi = {6'b101010, eac, bac, cnx, cnr};
assign bis = pdo[1:0];
assign eis = pdo[9:8];
assign xis = pdo[17:16];
assign ris = pdo[24];
assign brq = pdo[64];
assign erq = pdo[65];
assign ip0 = pdo[66];
assign ip1 = pdo[67];
assign tst = pdo[spi_size - 1];

ctr ctr(!n_rst, clk, rfc, ina, inb, bis, eis, brq, erq, bac, eac, xis, cnx, ris, cnr, bip, eip, bin, ein, ip0, ip1);
spi spi(!n_rst, clk, sck, sdi, sdo, !n_scs, pdi, pdo);

endmodule
