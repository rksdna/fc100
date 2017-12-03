/*
 * Serial peripheral interface
 *
 * rst - async reset
 * clk - main clock
 *
 * sck - serial clock
 * sdi - serial data input
 * sdo - serial data output
 * scs - serial select
 *
 * pdi - parallel data input
 * pdo - parallel data output
 */
 
module spi(rst, clk, sck, sdi, sdo, scs, pdi, pdo);

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
reg [1:0] xck;
reg [1:0] xcs;
reg xdi;

always @(posedge clk or posedge rst)
begin
    if (rst)
    begin
        sdo <= 1'b0;
        xck <= 2'b0;
        xcs <= 2'b0;
	    xdi <= 1'b0;
        pdo <= 0;
    end
    else
    begin
        xck <= {xck[0], sck};
        xcs <= {xcs[0], scs};
	    xdi <= sdi;
               
        if (xcs[0] && !xcs[1])
        begin         
            xrg <= pdi;
            sdo <= pdi[size - 1];
        end
        
        if (!xcs[0] && xcs[1])
        begin
           pdo <= xrg;  
        end
        
        if (xcs[0] && xck[0] && !xck[1])
        begin
            xrg[size - 1:0] <= {xrg[size - 2:0], xdi};
        end
        
        if (xcs[0] && !xck[0] && xck[1])
        begin
            sdo <= xrg[size - 1]; 
        end
    end
end

endmodule

