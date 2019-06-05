`timescale 1ns / 1ps


/*
Hussain Khajanchi 6/5/15

Overview: A simple 3x3 Matrix Multiplier module in SystemVerilog 

Input Parameters: 
               - clk     : a 100 MHz Clock based on a running Digilent NEXYS4DDR Board
               - rst     : active-low Synchronous Reset
               - mult_en : a control signal to enable matrix multiplication
               - matrix_a_stream : a 144-bit wire vector containing 9 16-bit array elements in top down order
               - matrix_b_stream : a 144-bit wire vector cotaining 9 16-bit array elements in top down order

Output Parameters: 
              - matrix_c_stream : a 144-bit wire vector containing 9 16-bit array elements in top down order

Methodology
    - Since SystemVerilog Array input parameters are not synthesizable, I had to "pack" the array into a 144-bit wire vector that contained
      all the elements of the array. 
      
      The standard 3x3 Matrix looks like this:
      
      matrix_a_unpacked = [ a1  a2  a3
                            a4  a5  a6 
                            a7  a8  a9]
                            
     I packed the elements in big-endian order (smallest to largest) in the 144-bit array. 
     The resulting array element map looks like this: 
     
     matrix_a_stream = [a1,a2,a3,a4,a5,a6,a7,a8,a9]
     
     I did the same thing for Matrix B. 
     
     To conduct the matrix multitplication, I decomposed the "matrix_a_stream"  into row vectors and the 
     "matrix_b_stream" into column vectors. Since each element in the resulting matrix is a function of the row vectors of Matrix A 
     and the Column Vectors of matrix B, the decomposing factor made it pretty easy to implement (albeit naive).
     
     Row Vector 1 = [a1 a2 a3]
     Row Vector 2 = [a4 a5 a6]
     Row Vector 3 = [a7 a8 a9]
     
     The Column Vectors look like this 
     Column Vector 1 = [b1 b4 b7]
     Column Vector 2 = [b2 b5 b8]         
     Column Vector 3 = [b3 b6 b9]
        
    The resulting matrix multiplication then goes to simple row/column vector multiplication (element-wise)
    i.e Matrix C (0,0) = Row Vector 1 * Column Vector 1  ---> RV1[0]*CV1[0] + RV1[1]*CV1[1] + RV1[2]*CV1[2]

Notes 
    - This is a super naive implementation, but future work would include making this parameterizable and scalable
    - This implementation can compute the new matrix in one clock cycle, but it uses a significant amount of DSP48 Blocks
      and may need to be pipelined for larger matricies
    - I plan on using this to use as a benchmark for High-Level Synthesis

*/



module mat_mult(clk,rst,mult_en, matrix_a_stream,matrix_b_stream,matrix_c_stream);

input logic clk; 
input logic rst; 
input logic mult_en; 

input [143:0] matrix_a_stream; 
input [143:0] matrix_b_stream;
output reg [143:0] matrix_c_stream;


//Create Unpacked SystemVerilog Matrix, not necessary for synthesis but nice for validation
reg [15:0] matrix_c [0:2][0:2];

//Instantiated Row and Column Vectors, 3 elements per vector 
reg [15:0] row_vect_a1 [0:2]; 
reg [15:0] row_vect_a2 [0:2]; 
reg [15:0] row_vect_a3 [0:2];

reg [15:0] col_vect_b1 [0:2];
reg [15:0] col_vect_b2 [0:2]; 
reg [15:0] col_vect_b3 [0:2];
 
 
 always_ff @ (posedge clk) //Flip-Flops for holding row/col vector data
    begin
        if (rst) 
            begin
                row_vect_a1 <= '{default:0};
                row_vect_a2 <= '{default:0};
                row_vect_a3 <= '{default:0};
                
                col_vect_b1  <= '{default:0};
                col_vect_b2  <= '{default:0};
                col_vect_b3  <= '{default:0};
                
            end 
       else 
            begin
                 row_vect_a1 <= '{ '{matrix_a_stream[143:128]},'{matrix_a_stream[127:112]}, '{matrix_a_stream[111:96]}};
                 row_vect_a2 <= '{ '{matrix_a_stream[95:80]},  '{matrix_a_stream[79:64]},   '{matrix_a_stream[63:48]} };
                 row_vect_a3 <= '{ '{matrix_a_stream[47:32]},  '{matrix_a_stream[31:16]},   '{matrix_a_stream[15:0]}  };
                 
                 col_vect_b1  <= '{ '{matrix_b_stream[143:128]},'{matrix_b_stream[95:80]},'{matrix_b_stream[47:32]} };
                 col_vect_b2  <= '{ '{matrix_b_stream[127:112]},'{matrix_b_stream[79:64]},'{matrix_b_stream[31:16]} };
                 col_vect_b3  <= '{ '{matrix_b_stream[111:96]} ,'{matrix_b_stream[63:48]},'{matrix_b_stream[15:0]}  };
            end 

    end 
    
    
always_ff @ (posedge clk) //Matrix Multiplication Register, done in 1 clock cycle
    begin
        if (rst)
            matrix_c <= '{default:0};
        else 
            begin
                if (mult_en)
                    begin
                    matrix_c[0][0] <= row_vect_a1[0]*col_vect_b1[0] + row_vect_a1[1]*col_vect_b1[1] + row_vect_a1[2]*col_vect_b1[2];
                    matrix_c[0][1] <= row_vect_a1[0]*col_vect_b2[0] + row_vect_a1[1]*col_vect_b2[1] + row_vect_a1[2]*col_vect_b2[2];
                    matrix_c[0][2] <= row_vect_a1[0]*col_vect_b3[0] + row_vect_a1[1]*col_vect_b3[1] + row_vect_a1[2]*col_vect_b3[2];
                    
                    matrix_c[1][0] <= row_vect_a2[0]*col_vect_b1[0] + row_vect_a2[1]*col_vect_b1[1] + row_vect_a2[2]*col_vect_b1[2];
                    matrix_c[1][1] <= row_vect_a2[0]*col_vect_b2[0] + row_vect_a2[1]*col_vect_b2[1] + row_vect_a2[2]*col_vect_b2[2];
                    matrix_c[1][2] <= row_vect_a2[0]*col_vect_b3[0] + row_vect_a2[1]*col_vect_b3[1] + row_vect_a2[2]*col_vect_b3[2];
 
                    matrix_c[2][0] <= row_vect_a3[0]*col_vect_b1[0] + row_vect_a3[1]*col_vect_b1[1] + row_vect_a3[2]*col_vect_b1[2];
                    matrix_c[2][1] <= row_vect_a3[0]*col_vect_b2[0] + row_vect_a3[1]*col_vect_b2[1] + row_vect_a3[2]*col_vect_b2[2];
                    matrix_c[2][2] <= row_vect_a3[0]*col_vect_b3[0] + row_vect_a3[1]*col_vect_b3[1] + row_vect_a3[2]*col_vect_b3[2];                   
                    
                    end
                else 
                    matrix_c <= matrix_c; 
           end
    end 
    
always_comb //Packs the matrix_c into a wire bus with the same big-endian method as matrix a and matrix b
    matrix_c_stream = {matrix_c[0][0], matrix_c[0][1], matrix_c[0][2], matrix_c[1][0], matrix_c[1][1], matrix_c[1][2], matrix_c[2][0], matrix_c[2][1], matrix_c[2][2]};
       
endmodule
