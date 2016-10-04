
.section .text
.global start

start:

ldr r0, =sbss
ldr r1, =ebss
and r2,r2,#0

wiper:
	cmp r0,r1
	beq endwiper
	str r2,[r0]
	add r0,r0,#1
	b wiper
endwiper:

ldr sp,=stack
b kmain


.section .data

.global stack
    .rept 1024
    .word 0
    .endr
stack:
    
