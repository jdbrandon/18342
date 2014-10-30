/* strTable.c
   Jeff Brandon
   jdbrando@andrew.cmu.edu
   9.20.2014
   18-342: Fundamentals of Embedded Systems
*/
	.file	"part1-strTable.c"
	.text
	.align	2
	.global	strTable
	.type	strTable, %function
strTable:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6}
	mov	r4, r3 
	cmp	r4, #0
	blt	.L11
	ldrb	r5, [r1, r4]	@ zero_extendqisi2
	subs	ip, r5, #23
	subges	ip, ip, #23
	subges	ip, ip, #23
	cmp	ip, #22
	subgt	pc, pc, #20
	cmp	ip, r2
	addgt	pc, pc, #12
	ldrb	r5, [r0, ip]	@ zero_extendqisi2
	ldrb	r6, [r1, r4]	@ zero_extendqisi2
	cmp	r5, r6
	strneb	r6, [r0, ip]
	subs	r4, r4, #1
	subge	pc, pc, #60
.L11:
	ldmfd	sp!, {r4, r5, r6}
	mov pc, lr
	.size	strTable, .-strTable
	.ident	"GCC: (GNU) 3.4.5"
