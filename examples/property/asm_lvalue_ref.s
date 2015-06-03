	.file	"asm.cpp"
	.text
	.globl	__Z2ggx
	.def	__Z2ggx;	.scl	2;	.type	32;	.endef
__Z2ggx:
LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	movl	8(%ebp), %eax
	movl	%eax, -8(%ebp)
	movl	12(%ebp), %eax
	movl	%eax, -4(%ebp)
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE0:
	.globl	__Z31CAIIIIIIIIIIIIIIIIIIIIIIIIIIIIOv
	.def	__Z31CAIIIIIIIIIIIIIIIIIIIIIIIIIIIIOv;	.scl	2;	.type	32;	.endef
__Z31CAIIIIIIIIIIIIIIIIIIIIIIIIIIIIOv:
LFB1:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	movl	$111111, (%esp)
	movl	$0, 4(%esp)
	call	__Z2ggx
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE1:
	.ident	"GCC: (GNU) 4.8.1"
