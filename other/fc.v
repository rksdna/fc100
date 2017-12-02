/* 
 * Frequency meter    
 */ 
 
`include "spi.v"
`include "ctr.v"

module fc(clk, _rst, ina, inb, sck, sdi, sdo, _scs, bip, eip, bin, ein);

parameter ctr_size = 32;
parameter spi_size = 2 * ctr_size + 2;

input wire clk;
input wire _rst;

input wire ina;
input wire inb;

input wire sck;
input wire sdi;
output wire sdo;
input wire _scs;

output wire bip;
output wire eip;

output wire bin;
output wire ein;

wire rst;
wire scs;

wire [1:0] bis;
wire [1:0] eis;
wire brq;
wire erq;
wire ip0;
wire ip1;
wire bac;
wire eac;

wire [ctr_size - 1:0] cta;
wire [ctr_size - 1:0] ctc;

wire [spi_size - 1:0] pdi;
wire [spi_size- 1:0] pdo;

defparam ctr.size = ctr_size;
defparam spi.size = spi_size;

assign rst = !_rst;
assign scs = !_scs;
assign pdi = {eac, bac, cta, ctc};
assign bis = pdo[7:6];
assign eis = pdo[5:4];
assign brq = pdo[3];
assign erq = pdo[2];
assign ip1 = pdo[1];
assign ip0 = pdo[0];

ctr ctr(clk, rst, ina, inb, bis, eis, brq, erq, bac, eac, cta, ctc, bip, eip, bin, ein, ip0, ip1);
spi spi(clk, rst, sck, sdi, sdo, scs, pdi, pdo);

endmodule
