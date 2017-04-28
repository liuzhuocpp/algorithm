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
	subq	$16, %rsp
.Lcfi1:
	.seh_stackalloc 16
.Lcfi2:
	.seh_endprologue
	movl	%ecx, 12(%rsp)
	movl	$22222, 8(%rsp)         # imm = 0x56CE
	movl	$33333, 4(%rsp)         # imm = 0x8235
.LBB0_1:                                # %while.cond
                                        # =>This Inner Loop Header: Depth=1
	cmpl	$0, 12(%rsp)
	jle	.LBB0_3
# BB#2:                                 # %while.body
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	$555555, (%rsp)         # imm = 0x87A23
	movl	(%rsp), %eax
	addl	4(%rsp), %eax
	movl	%eax, 4(%rsp)
	jmp	.LBB0_1
.LBB0_3:                                # %while.end
	movl	4(%rsp), %eax
	addl	8(%rsp), %eax
	movl	%eax, 8(%rsp)
	movl	8(%rsp), %eax
	addq	$16, %rsp
	retq
	.seh_handlerdata
	.text
.Lcfi3:
	.seh_endproc

	.def	 g;
	.scl	2;
	.type	32;
	.endef
	.globl	g
	.p2align	4, 0x90
g:                                      # @g
.Lcfi4:
.seh_proc g
# BB#0:                                 # %entry
	subq	$40, %rsp
.Lcfi5:
	.seh_stackalloc 40
.Lcfi6:
	.seh_endprologue
	movl	%ecx, 36(%rsp)
	movl	36(%rsp), %ecx
	callq	f
	nop
	addq	$40, %rsp
	retq
	.seh_handlerdata
	.text
.Lcfi7:
	.seh_endproc

	.comm	a1,4,2                  # @a1
	.comm	a2,4,2                  # @a2
	.comm	a3,4,2                  # @a3
	.comm	a4,4,2                  # @a4
	.comm	a5,4,2                  # @a5

