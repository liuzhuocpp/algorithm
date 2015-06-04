	.file	"asm.cpp"
	.text
	.globl	__Z31CAIIIIIIIIIIIIIIIIIIIIIIIIIIIIOv
	.def	__Z31CAIIIIIIIIIIIIIIIIIIIIIIIIIIIIOv;	.scl	2;	.type	32;	.endef
__Z31CAIIIIIIIIIIIIIIIIIIIIIIIIIIIIOv:
LFB17:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$496, %esp
	movl	$1, -24(%ebp)
	movl	$2, -20(%ebp)
	movl	$3, -16(%ebp)
	movl	$4, -12(%ebp)
	movl	$5, -8(%ebp)
	movl	$6, -4(%ebp)
	movl	-24(%ebp), %eax
	movl	%eax, -48(%ebp)
	movl	-20(%ebp), %eax
	movl	%eax, -44(%ebp)
	movl	-16(%ebp), %eax
	movl	%eax, -40(%ebp)
	movl	-12(%ebp), %eax
	movl	%eax, -36(%ebp)
	movl	-8(%ebp), %eax
	movl	%eax, -32(%ebp)
	movl	-4(%ebp), %eax
	movl	%eax, -28(%ebp)
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE17:
	.ident	"GCC: (GNU) 4.8.1"
