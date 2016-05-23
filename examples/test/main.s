	.file	"main.cpp"
	.globl	A
	.bss
	.align 4
A:
	.space 4
	.globl	B
	.align 4
B:
	.space 4
	.text
	.globl	_Z4testv
	.def	_Z4testv;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z4testv
_Z4testv:
.LFB0:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.seh_endprologue
	movl	B(%rip), %eax
	addl	$1, %eax
	movl	%eax, A(%rip)
	movl	$0, B(%rip)
	nop
	popq	%rbp
	ret
	.seh_endproc
	.ident	"GCC: (tdm64-1) 5.1.0"
