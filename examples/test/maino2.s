	.file	"main.cpp"
	.text
	.p2align 4,,15
	.globl	_Z4testv
	.def	_Z4testv;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z4testv
_Z4testv:
.LFB0:
	.seh_endprologue
	movl	B(%rip), %eax
	movl	$0, B(%rip)
	addl	$1, %eax
	movl	%eax, A(%rip)
	ret
	.seh_endproc
	.globl	B
	.bss
	.align 4
B:
	.space 4
	.globl	A
	.align 4
A:
	.space 4
	.ident	"GCC: (tdm64-1) 5.1.0"
