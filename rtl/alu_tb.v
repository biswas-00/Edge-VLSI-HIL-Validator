`timescale 1ns/1ps

module alu_tb;
    reg [3:0] A, B;
    reg [1:0] opcode;
    wire [3:0] result;
    wire carry;

    // Instantiate the ALU
    alu dut (
        .A(A), 
        .B(B), 
        .opcode(opcode), 
        .result(result), 
        .carry(carry)
    );

    initial begin
        // Use macros passed from the Python bridge
        // Note the backticks (`) before the macro names
        `ifdef VAL_A
            A = `VAL_A; 
            B = `VAL_B; 
            opcode = `OP_CODE;
        `else
            A = 0; B = 0; opcode = 0;
        `endif

        #1; // Wait for logic to settle
        $display("%d", result); // Output only the result for Python to read
        $finish;
    end
endmodule