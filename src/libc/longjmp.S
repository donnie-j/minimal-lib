.global __longjmp
.global _longjmp
.type   __longjmp, @function
.type   _longjmp, @function
__longjmp:
_longjmp:
	mov.l  @r4+, r8
	mov.l  @r4+, r9
	mov.l  @r4+, r10
	mov.l  @r4+, r11
	mov.l  @r4+, r12
	mov.l  @r4+, r13
	mov.l  @r4+, r14
	mov.l  @r4+, r15
	lds.l  @r4+, pr

	tst  r5, r5
	movt r0
	add  r5, r0

	rts
	 nop
