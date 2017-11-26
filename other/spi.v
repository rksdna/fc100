
module spi(nrst, clk, sck, mosi, miso, nss, di, do, we, re);

parameter size = 8;

input wire nrst;
input wire clk; 

input wire sck;
input wire mosi;
output wire miso;
input wire nss;

input wire [size - 1:0] di;
output wire [size - 1:0] do;
output wire we;
output wire re;

reg [1:0] sck_latch;
reg mosi_latch;
reg miso_latch;
reg [1:0] nss_latch;
reg [size - 1:0] shift_latch;

wire nss_fall;
wire nss_busy;
wire nss_rise;

wire sck_rise;
wire sck_fall;

assign nss_fall = nss_latch == 2'b10;
assign nss_busy = nss_latch == 2'b00;
assign nss_rise = nss_latch == 2'b01;

assign sck_rise = sck_latch == 2'b01 && nss_busy;
assign sck_fall = sck_latch == 2'b10 && nss_busy;

assign miso = nss_busy ? miso_latch : 1'bZ;
assign re = nss_fall;
assign we = nss_rise;
assign do = we ? shift_latch : {size{1'bZ}};

always @(posedge clk or negedge nrst)
begin
    if (nrst)
    begin
        sck_latch[1:0] <= {sck_latch[0], sck};
        nss_latch[1:0] <= {nss_latch[0], nss};            
        mosi_latch <= mosi;

        if (nss_fall)
        begin
            shift_latch[size - 1:0] <= di[size - 1:0];
            miso_latch <= di[size - 1];
        end

        if (nss_busy && sck_rise)
        begin
            shift_latch[size - 1:0] = {shift_latch[size - 2:0], mosi_latch};
        end

        if (nss_busy && sck_fall)
        begin
            miso_latch <= shift_latch[size - 1]; 
        end

    end
    else
    begin
        sck_latch <= 2'b00;
        miso_latch <= 1'b0;
        mosi_latch <= 1'b0;
        nss_latch <= 2'b11;
        shift_latch <= {size{1'b0}};
    end
end

endmodule

