module alu(
    input [15:0] A, B,      // Upgraded to 16-bit
    input [1:0] opcode,     
    output reg [15:0] result,
    output reg carry,
    output zero,            // Flag: True if result is 0
    output negative         // Flag: True if result is negative (MSB is 1)
);

// Combinational logic for flags
assign zero = (result == 16'b0);
assign negative = result[15]; // The Most Significant Bit indicates sign

always @(*) begin
    case(opcode)
        2'b00: {carry, result} = A + B; // 16-bit Addition
        2'b01: {carry, result} = A - B; // 16-bit Subtraction
        2'b10: begin result = A & B; carry = 0; end
        2'b11: begin result = A | B; carry = 0; end
        default: begin result = 16'b0; carry = 0; end
    endcase
end

endmodule