/*
 * Serial peripheral interface
 *
 * clk - reference clock
 * rst - async reset
 *
 * sck - serial clock
 * sdi - serial data input
 * sdo - serial data output
 * scs - serial select
 *
 * pdi - parallel data input
 * pdo - parallel data output
 *
 */
 
module spi(clk, rst, sck, sdi, sdo, scs, pdi, pdo);

parameter size = 8;

input wire rst;
input wire clk; 

input wire sck;
input wire sdi;
output reg sdo;
input wire scs;

input wire [size - 1:0] pdi;
output reg [size - 1:0] pdo;

reg [size - 1:0] xrg;
reg xck;
reg xcs;

always @(posedge clk or posedge rst)
begin
    if (rst)
    begin
        sdo <= 1'b0;
        xck <= 1'b0;
        xcs <= 1'b0;
        pdo <= 0;
    end
    else
    begin
        xck <= sck;
        xcs <= scs;
               
        if (scs && !xcs)
        begin         
            xrg <= pdi;
            sdo <= pdi[size - 1];
        end
        
        if (!scs && xcs)
        begin
           pdo <= xrg;  
        end
        
        if (scs && sck && !xck)
        begin
            xrg[size - 1:0] <= {xrg[size - 2:0], sdi};
        end
        
        if (scs && !sck && xck)
        begin
            sdo <= xrg[size - 1]; 
        end
    end
end

endmodule

