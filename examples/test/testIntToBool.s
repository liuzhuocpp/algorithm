	.text
	.def	 _Z1fi;
	.scl	2;
	.type	32;
	.endef
	.globl	_Z1fi
	.p2align	4, 0x90
_Z1fi:                                  # @_Z1fi
.Lcfi0:
.seh_proc _Z1fi
# BB#0:                                 # %entry
	pushq	%rax
.Lcfi1:
	.seh_stackalloc 8
.Lcfi2:
	.seh_endprologue
	movl	%ecx, 4(%rsp)
	cmpl	$0, 4(%rsp)
	setne	%al
	andb	$1, %al
	movb	%al, 3(%rsp)
	movb	3(%rsp), %al
	andb	$1, %al
	movzbl	%al, %eax
	popq	%rcx
	retq
	.seh_handlerdata
	.text
.Lcfi3:
	.seh_endproc


