`timescale 1ns/1ps

module test_tb;

    // Inputs
    logic clk;
    logic rst;
    logic in1;
    logic in2;
    logic out_take;

    // Outputs
    logic out1;

    // Instantiate the Unit Under Test (UUT)
    test uut (
        .clk(clk),
        .rst(rst),
        .in1(in1),
        .in2(in2),
        .out_take(out_take),
        .out1(out1)
    );

    // Clock generation: 10ns clock period (50MHz)
    always #5 clk = ~clk;

    // Simulation stimulus
    initial begin
        // Initialize inputs
        clk = 0;
        rst = 1;
        in1 = 0;
        in2 = 0;
        out_take = 0;

        // Apply reset
        #10;
        rst = 0;
        // Dump waveforms to VCD
        $dumpfile("test.vcd");
        $dumpvars(0, test_tb);
       
        // Stimulate inputs
        #10 in1 = 1; in2 = 1;  // Both in1 and in2 are 1, t1 will be 1
        #20 in1 = 0; in2 = 0;  // t1 becomes 0
        #10 in1 = 1; in2 = 1;  // Both in1 and in2 are 1, t1 will be 1
        #20 in1 = 0; in2 = 0;  // t1 becomes 0
        #20 out_take = 1;      // Activate out_take signal

        // Keep stimulating transitions
        // End simulation after some time
	#100 $finish;


    end

endmodule

