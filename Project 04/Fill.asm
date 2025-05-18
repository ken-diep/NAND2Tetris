// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

//// Replace this comment with your code.

(START)

@8191
D=A

@n
M=D
@i
M=0

@KBD
D=M
@LOOP
D;JNE
@BLANK
D;JEQ

@START
0;JMP

(LOOP)

//Break condition
@n
D=M
@i
D=M-D
@START
D;JGT

//Write 1 to every word

//256 rows x 512 columns
//32 words

@i
D=M
@SCREEN
A=D+A
M=-1
@i
M=M+1

@LOOP
0;JMP

(BLANK)
//Write 0 to every word
//Break condition

@KBD
D=M
@START
D;JNE

@n
D=M
@i
D=M-D
@START
D;JGT

//Write 0 to every word

//256 rows x 512 columns
//32 words

@i
D=M
@SCREEN
A=D+A
M=0
@i
M=M+1

@BLANK
0;JMP

(END)
@START
0;JMP

