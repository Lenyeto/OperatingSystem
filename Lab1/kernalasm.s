

;ldr r1, 4500 ;Should be 5
;ldr r1, 2016 ;Should be 6
;ldr r1, 0 ;Should be 0

	ldr r1,#2016
	ldr r2,#0
	
b count
	
forever:
	b forever
	
	
count:
	cmp r1,#0
	b forever
	mov r0, #0
	add r0, r1, lsl#2
	add r2, r0
	
	b count