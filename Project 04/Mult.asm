// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// The algorithm is based on repetitive addition.

//// Get value from R0
//For i < R1, add R0 to itself

@i
M=1

@R2
M=0

(LOOP)
//Break condition
    @i
    D=M

    @R1
    D=D-M

    @END
    D;JGT //if i > R1, break

    @R0
    D=M

    @R2
    M=D+M //Add R0 to R2

    @i
    M=M+1

    @LOOP
    0;JMP //Loop back

//End loop
(END)
@END
0;JMP
