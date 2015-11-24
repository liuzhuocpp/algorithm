	.file	"main.cpp"
	.text
	.globl	_Z1fv
	.def	_Z1fv;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z1fv
_Z1fv:
.LFB1:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$16, %rsp
	.seh_stackalloc	16
	.seh_endprologue
	nop
	addq	$16, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	_Z1gv
	.def	_Z1gv;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z1gv
_Z1gv:
.LFB2:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.seh_endprologue
	nop
	popq	%rbp
	ret
	.seh_endproc
	.ident	"GCC: (tdm64-1) 5.1.0"
