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
	movl	$576868, 4(%rsp)        # imm = 0x8CD64
	popq	%rax
	retq
	.seh_handlerdata
	.text
.Lcfi3:
	.seh_endproc


