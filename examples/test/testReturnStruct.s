	.file	"testReturnStruct.c"
	.comm	a, 4, 2
	.comm	b, 4, 2
	.comm	c, 4, 2
	.comm	d, 4, 2
	.comm	e, 4, 2
	.comm	h, 4, 2
	.comm	g, 4, 2
	.text
	.globl	f
	.def	f;	.scl	2;	.type	32;	.endef
	.seh_proc	f
f:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movl	$188888, -32(%rbp)
	movl	$288888, -28(%rbp)
	movl	$388888, -24(%rbp)
	movl	$488888, -20(%rbp)
	movq	$588888, -16(%rbp)
	movq	$688888, -8(%rbp)
	movq	16(%rbp), %rax
	movq	-32(%rbp), %rdx
	movq	%rdx, (%rax)
	movq	-24(%rbp), %rdx
	movq	%rdx, 8(%rax)
	movq	-16(%rbp), %rdx
	movq	%rdx, 16(%rax)
	movq	-8(%rbp), %rdx
	movq	%rdx, 24(%rax)
	movq	16(%rbp), %rax
	addq	$32, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	ggg
	.def	ggg;	.scl	2;	.type	32;	.endef
	.seh_proc	ggg
ggg:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$64, %rsp
	.seh_stackalloc	64
	.seh_endprologue
	leaq	-32(%rbp), %rax
	movq	%rax, %rcx
	call	f
	movl	$999, -32(%rbp)
	movl	$1999, -28(%rbp)
	movl	$2999, -24(%rbp)
	movl	$3999, -20(%rbp)
	movq	$22222, -16(%rbp)
	movq	$55555555, -8(%rbp)
	nop
	addq	$64, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	f2
	.def	f2;	.scl	2;	.type	32;	.endef
	.seh_proc	f2
f2:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	16(%rbp), %rax
	movq	-32(%rbp), %rdx
	movq	%rdx, (%rax)
	movq	-24(%rbp), %rdx
	movq	%rdx, 8(%rax)
	movq	-16(%rbp), %rdx
	movq	%rdx, 16(%rax)
	movq	-8(%rbp), %rdx
	movq	%rdx, 24(%rax)
	movq	16(%rbp), %rax
	addq	$32, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	dddd
	.def	dddd;	.scl	2;	.type	32;	.endef
	.seh_proc	dddd
dddd:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$64, %rsp
	.seh_stackalloc	64
	.seh_endprologue
	leaq	-32(%rbp), %rax
	movq	%rax, %rcx
	call	f
	call	ggg
	leaq	-32(%rbp), %rax
	movq	%rax, %rcx
	call	f2
	nop
	addq	$64, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.ident	"GCC: (tdm64-1) 5.1.0"
