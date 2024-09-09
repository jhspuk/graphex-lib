module test(
	input logic clk,
	input logic rst,

	input logic in1,
	input logic in2,
	input logic out_take,
	output logic out1
);

logic p1;
logic p2;
logic p3;
logic p4;
logic p5;
logic p6;
logic p7;
logic p8;
logic p9;
logic p10;

initial p1 = 0;
initial p2 = 0;
initial p3 = 0;
initial p4 = 0;
initial p5 = 0;
initial p6 = 0;
initial p7 = 0;
initial p8 = 0;
initial p9 = 0;
initial p10 = 0;

wire t1;
wire t2;
wire t3;
wire t4;
wire t5;
wire t6;
wire t7;
wire t8;
wire t9;
wire t10;

assign t1 = in1 & in2;
assign t2 = p1;
assign t3 = p2;
assign t4 = p3;
assign t5 = p4;
assign t6 = p5;
assign t7 = p6;
assign t8 = p7;
assign t9 = p8;
assign t10 = p9;

always_ff@(posedge clk or posedge rst) begin
	if(rst) begin
		p1 <= 0;
		p2 <= 0;
		p3 <= 0;
		p4 <= 0;
		p5 <= 0;
		p6 <= 0;
		p7 <= 0;
		p8 <= 0;
		p9 <= 0;
		p10 <= 0;
		out1 <= 0;
	end else begin
		p1 <= p1 & ~t2 || t1;
		p2 <= p2 & ~t3 || t2;
		p3 <= p3 & ~t4 || t3;
		p4 <= p4 & ~t5 || t4;
		p5 <= p5 & ~t6 || t5;
		p6 <= p6 & ~t7 || t6;
		p7 <= p7 & ~t8 || t7;
		p8 <= p8 & ~t9 || t8;
		p9 <= p9 & ~t10 || t9;
		out1 <= out1 & ~out_take || t10;
	end
end

endmodule

