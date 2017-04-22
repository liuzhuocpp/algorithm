	.text
	.def	 _Z1fii;
	.scl	2;
	.type	32;
	.endef
	.globl	_Z1fii
	.p2align	4, 0x90
_Z1fii:                                 # @_Z1fii
.Lcfi0:
.seh_proc _Z1fii
# BB#0:                                 # %entry
	pushq	%rbp
.Lcfi1:
	.seh_pushreg 5
	subq	$32, %rsp
.Lcfi2:
	.seh_stackalloc 32
	leaq	32(%rsp), %rbp
.Lcfi3:
	.seh_setframe 5, 32
.Lcfi4:
	.seh_endprologue
	movl	%ecx, -4(%rbp)
	movl	%edx, -8(%rbp)
	movl	-4(%rbp), %ecx
	movl	%ecx, %eax
	movl	-8(%rbp), %ecx
	movl	%ecx, %r8d
	movq	%rsp, %r9
	movq	%r9, -16(%rbp)
	imulq	%r8, %rax
	leaq	15(,%rax,4), %rax
	andq	$-16, %rax
	movq	%r8, -24(%rbp)          # 8-byte Spill
	callq	___chkstk_ms
	subq	%rax, %rsp
	movq	%rsp, %rax
	movq	-24(%rbp), %r8          # 8-byte Reload
	shlq	$1, %r8
	movl	$11, 12(%rax,%r8,4)
	movq	-16(%rbp), %rax
	movq	%rax, %rsp
	movq	%rbp, %rsp
	popq	%rbp
	retq
	.seh_handlerdata
	.text
.Lcfi5:
	.seh_endproc


