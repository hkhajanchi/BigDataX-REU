`timescale 1ns / 1ps
/*
    Hussain Khajanchi - 6/5/19
    
    Overview: 
        - SystemVerilog Testbench for mat_mult.sv
        - Drives the unit-under-test with two packed matrices 
        
       Matrix A = [ 1 2 3
                    4 5 6
                    7 8 9]
       Matrix B = [ 10 11 12
                    13 14 15 
                    16 17 18]
                     
    The resulting matrix should be
    
      Matrix C = [  84  90  96
                   201 216 231
                   318 342 366]
       
       **Verified using MATLAB**


*/

module mat_mult_tb();

reg clk; 
reg rst; 
reg mult_en; 
reg [143:0] matrix_a;
reg [143:0] matrix_b;


mat_mult uut 
(
.clk(clk),
.rst(rst),
.mult_en(mult_en),
.matrix_a_stream(matrix_a),
.matrix_b_stream(matrix_b)
); 

//Generate 100 MHz Clock
initial forever begin
clk = 0; 
#5; 
clk = 1; 
#5; 
end 

//Synchronous Reset Signal
initial begin
rst = 1; 
#1000; 
rst = 0; 
end 

//Enable signal for matrix multiplication
initial begin
mult_en = 0; 
#1100; 
mult_en = 1; 
end 

//Uses flip-flop logic to drive the matrix_a and matrix_b signals

always_ff @ (posedge clk) 
begin
    if (rst)
        begin
        matrix_a <= 143'b0;
        matrix_b <= 143'b0; 
        end 
   else 
        begin
        matrix_a <= {16'd1,16'd2,16'd3,16'd4,16'd5,16'd6,16'd7,16'd8,16'd9};
        matrix_b <= {16'd10,16'd11,16'd12,16'd13,16'd14,16'd15,16'd16,16'd17,16'd18};
        end 
end        






endmodule
