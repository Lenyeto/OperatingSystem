ldr r1,=#4500
mov r2,#0
mov r3,#0
mov r4,#1
b count
forever:
	b forever
count:
	cmp r1,#0
	beq forever
	and r4, r1, #1
	cmp r4,#1
	addeq r0, r0, #1
	add r1, r3, r1, lsr#1
	b count