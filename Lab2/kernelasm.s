ldr r1,=#4500
count:
	movs r1, r1, LSL #1
	addcs r0 r0, #1
	cmp r1,#0
	bne count
forever:
	b forever