/*
 * Memory serial peripheral interface
 *
 * rst - async reset
 *
 * clk - system clock
 *
 * spi_clk - serial clock
 * spi_mosi - serial data input
 * spi_miso - serial data output
 * spi_ss - serial frame sync
 *
 * mem_addr - address bus
 * mem_din - data input
 * mem_dout - data output
 * mem_wen - write strobe
 */
 
module spi(rst, clk, spi_clk, spi_mosi, spi_miso, spi_ss, mem_addr, mem_din, mem_dout, mem_wrt);

input wire rst;

input wire clk;

input wire spi_clk;
input wire spi_mosi;
output wire spi_miso;
input wire spi_ss;

output wire [3:0] mem_addr;
input wire [7:0] mem_din;
output wire [7:0] mem_dout;
output wire mem_wrt;

reg [1:0] sync_spi_clk;
reg sync_spi_ss;
reg sync_spi_mosi;

reg den_reg;
reg wen_reg;
reg [3:0] addr_reg;

reg mosi_reg;
reg [7:0] miso_reg;
reg [2:0] bit_cnt;

assign spi_miso = miso_reg[7];
assign mem_addr = addr_reg;
assign mem_dout = {miso_reg[6:0], sync_spi_mosi};
assign mem_wrt = sync_spi_clk[0] && !sync_spi_clk[1] && bit_cnt == 3'b111 && wen_reg;

always @(posedge clk or posedge rst)
begin
    if (rst)
    begin
        sync_spi_clk <= 2'b0;
        sync_spi_ss <= 1'b0;
        sync_spi_mosi <= 1'b0;

        den_reg <= 1'b0;
        wen_reg <= 1'b0;  
        addr_reg <= 4'b0;
              
        mosi_reg <= 1'b0;        
        miso_reg <= 8'b0;
        bit_cnt <= 3'b0;  
    end
    else
    begin
        sync_spi_clk <= {sync_spi_clk[0], spi_clk};
        sync_spi_ss <= spi_ss;
        sync_spi_mosi <= spi_mosi;
                
        // spi_ss active
        if (sync_spi_ss)
        begin     
            // spi_clk rise    
            if (sync_spi_clk[0] && !sync_spi_clk[1])
            begin      
                mosi_reg <= sync_spi_mosi;
                
                if (bit_cnt == 3'b111)
                begin
                    den_reg <= 1'b1;
                    wen_reg <= den_reg ? wen_reg : mem_dout[7];
                    addr_reg <= den_reg ? addr_reg + 1'b1 : mem_dout[3:0]; 
                end
            end

            // spi_clk fall
            if (!sync_spi_clk[0] && sync_spi_clk[1])
            begin
                bit_cnt <= bit_cnt + 1'b1;

                if (bit_cnt == 3'b111)
                    miso_reg <= mem_din;     
                else
                    miso_reg <= {miso_reg[6:0], mosi_reg};
            end
        end        
        else
        begin
            den_reg <= 1'b0;
            wen_reg <= 1'b0;  
            addr_reg <= 4'b0;
              
            mosi_reg <= 1'b0;        
            miso_reg <= 8'b0;
            bit_cnt <= 3'b0;        
        end
    end
end

endmodule

