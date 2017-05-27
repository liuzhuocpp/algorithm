	.text
	.def	 f;
	.scl	2;
	.type	32;
	.endef
	.globl	f
	.p2align	4, 0x90
f:                                      # @f
.Lcfi0:
.seh_proc f
# BB#0:                                 # %entry
	subq	$24, %rsp
.Lcfi1:
	.seh_stackalloc 24
.Lcfi2:
	.seh_endprologue
	movl	$-1817433241, 20(%rsp)  # imm = 0x93AC2B67
	movl	$-22, 16(%rsp)
	movl	20(%rsp), %eax
	cmpl	16(%rsp), %eax
	setg	%cl
	andb	$1, %cl
	movb	%cl, 15(%rsp)
	movl	$-333, 8(%rsp)          # imm = 0xFEB3
	movl	$-22, 4(%rsp)
	movl	20(%rsp), %eax
	cmpl	16(%rsp), %eax
	setg	%cl
	andb	$1, %cl
	movb	%cl, 3(%rsp)
	addq	$24, %rsp
	retq
	.seh_handlerdata
	.text
.Lcfi3:
	.seh_endproc

	.def	 main;
	.scl	2;
	.type	32;
	.endef
	.globl	main
	.p2align	4, 0x90
main:                                   # @main
.Lcfi4:
.seh_proc main
# BB#0:                                 # %entry
	pushq	%rax
.Lcfi5:
	.seh_stackalloc 8
.Lcfi6:
	.seh_endprologue
	xorl	%eax, %eax
	movl	$0, 4(%rsp)
	popq	%rcx
	retq
	.seh_handlerdata
	.text
.Lcfi7:
	.seh_endproc


