	.text
	.def	 _Z4testv;
	.scl	2;
	.type	32;
	.endef
	.section	.rdata,"dr"
	.p2align	3
LCPI0_0:
	.quad	4612136378390124954     # double 2.2000000000000002
LCPI0_1:
	.quad	4623113902481840538     # double 12.300000000000001
	.text
	.globl	_Z4testv
	.p2align	4, 0x90
_Z4testv:                               # @_Z4testv
.Ltmp0:
.seh_proc _Z4testv
# BB#0:
	subq	$16, %rsp
.Ltmp1:
	.seh_stackalloc 16
.Ltmp2:
	.seh_endprologue
	movsd	LCPI0_0(%rip), %xmm0    # xmm0 = mem[0],zero
	movsd	LCPI0_1(%rip), %xmm1    # xmm1 = mem[0],zero
	movl	B(%rip), %eax
	addl	$1, %eax
	movl	%eax, A(%rip)
	movl	$0, B(%rip)
	movsd	%xmm1, 8(%rsp)
	movsd	%xmm0, (%rsp)
	movsd	(%rsp), %xmm0           # xmm0 = mem[0],zero
	addsd	8(%rsp), %xmm0
	movsd	%xmm0, (%rsp)
	movl	A(%rip), %eax
	addl	B(%rip), %eax
	addq	$16, %rsp
	retq
	.seh_handlerdata
	.text
.Ltmp3:
	.seh_endproc

	.bss
	.globl	A                       # @A
	.p2align	2
A:
	.long	0                       # 0x0

	.globl	B                       # @B
	.p2align	2
B:
	.long	0                       # 0x0


