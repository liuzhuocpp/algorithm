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
	pushq	%rax
.Lcfi1:
	.seh_stackalloc 8
.Lcfi2:
	.seh_endprologue
	movl	%ecx, 4(%rsp)
	cmpl	$0, 4(%rsp)
	jle	.LBB0_2
# BB#1:                                 # %if.then
	movl	4(%rsp), %eax
	addl	$1, %eax
	movl	%eax, 4(%rsp)
	jmp	.LBB0_11
.LBB0_2:                                # %if.else
	cmpl	$-1, 4(%rsp)
	jle	.LBB0_4
# BB#3:                                 # %if.then2
	movl	4(%rsp), %eax
	addl	$-1, %eax
	movl	%eax, 4(%rsp)
	jmp	.LBB0_10
.LBB0_4:                                # %if.else3
	cmpl	$33, 4(%rsp)
	jne	.LBB0_6
# BB#5:                                 # %if.then5
	movl	4(%rsp), %eax
	addl	$3, %eax
	movl	%eax, 4(%rsp)
	jmp	.LBB0_9
.LBB0_6:                                # %if.else6
	cmpl	$55, 4(%rsp)
	jne	.LBB0_8
# BB#7:                                 # %if.then8
	movl	4(%rsp), %eax
	addl	$5, %eax
	movl	%eax, 4(%rsp)
.LBB0_8:                                # %if.end
	jmp	.LBB0_9
.LBB0_9:                                # %if.end10
	jmp	.LBB0_10
.LBB0_10:                               # %if.end11
	jmp	.LBB0_11
.LBB0_11:                               # %if.end12
	popq	%rax
	retq
	.seh_handlerdata
	.text
.Lcfi3:
	.seh_endproc

	.def	 gg;
	.scl	2;
	.type	32;
	.endef
	.globl	gg
	.p2align	4, 0x90
gg:                                     # @gg
.Lcfi4:
.seh_proc gg
# BB#0:                                 # %entry
	pushq	%rax
.Lcfi5:
	.seh_stackalloc 8
.Lcfi6:
	.seh_endprologue
	movl	%ecx, 4(%rsp)
	cmpl	$0, 4(%rsp)
	jle	.LBB1_2
# BB#1:                                 # %if.then
	movl	4(%rsp), %eax
	addl	$1, %eax
	movl	%eax, 4(%rsp)
	jmp	.LBB1_11
.LBB1_2:                                # %if.else
	cmpl	$-1, 4(%rsp)
	jle	.LBB1_4
# BB#3:                                 # %if.then2
	movl	4(%rsp), %eax
	addl	$-1, %eax
	movl	%eax, 4(%rsp)
	jmp	.LBB1_10
.LBB1_4:                                # %if.else3
	cmpl	$33, 4(%rsp)
	jne	.LBB1_6
# BB#5:                                 # %if.then5
	movl	4(%rsp), %eax
	addl	$3, %eax
	movl	%eax, 4(%rsp)
	jmp	.LBB1_9
.LBB1_6:                                # %if.else6
	cmpl	$55, 4(%rsp)
	jne	.LBB1_8
# BB#7:                                 # %if.then8
	movl	4(%rsp), %eax
	addl	$5, %eax
	movl	%eax, 4(%rsp)
.LBB1_8:                                # %if.end
	jmp	.LBB1_9
.LBB1_9:                                # %if.end10
	jmp	.LBB1_10
.LBB1_10:                               # %if.end11
	jmp	.LBB1_11
.LBB1_11:                               # %if.end12
	popq	%rax
	retq
	.seh_handlerdata
	.text
.Lcfi7:
	.seh_endproc


