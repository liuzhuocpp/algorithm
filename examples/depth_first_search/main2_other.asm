	.file	"main2.cpp"
	.text
	.globl	__Z2GGv
	.def	__Z2GGv;	.scl	2;	.type	32;	.endef
__Z2GGv:
LFB2:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE2:
	.align 2
	.def	__ZZ4testvENKUlvE_clEv;	.scl	3;	.type	32;	.endef
__ZZ4testvENKUlvE_clEv:
LFB5:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$20, %esp
	movl	%ecx, -20(%ebp)
	movl	$9998, -4(%ebp)
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE5:
	.align 2
	.def	__ZZ4testvENKUlvE0_clEv;	.scl	3;	.type	32;	.endef
__ZZ4testvENKUlvE0_clEv:
LFB8:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$20, %esp
	movl	%ecx, -20(%ebp)
	movl	$10010010, -4(%ebp)
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE8:
	.globl	__Z4testv
	.def	__Z4testv;	.scl	2;	.type	32;	.endef
__Z4testv:
LFB4:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%esi
	pushl	%ebx
	subl	$32, %esp
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	movb	$1, -9(%ebp)
	movb	%dl, 4(%esp)
	movb	%al, (%esp)
	call	__ZN6ChooseILb0EE3runIZ4testvEUlvE0_Z4testvEUlvE_EEvT_T0_
	movl	%esi, %eax
	movb	%al, 4(%esp)
	movb	%bl, (%esp)
	call	__ZN6ChooseILb0EE3runIZ4testvEUlvE_Z4testvEUlvE0_EEvT_T0_
	movl	$0, %eax
	addl	$32, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE4:
	.def	__ZN6ChooseILb0EE3runIZ4testvEUlvE0_Z4testvEUlvE_EEvT_T0_;	.scl	3;	.type	32;	.endef
__ZN6ChooseILb0EE3runIZ4testvEUlvE0_Z4testvEUlvE_EEvT_T0_:
LFB11:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	leal	12(%ebp), %eax
	movl	%eax, %ecx
	call	__ZZ4testvENKUlvE_clEv
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE11:
	.def	__ZN6ChooseILb0EE3runIZ4testvEUlvE_Z4testvEUlvE0_EEvT_T0_;	.scl	3;	.type	32;	.endef
__ZN6ChooseILb0EE3runIZ4testvEUlvE_Z4testvEUlvE0_EEvT_T0_:
LFB12:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	leal	12(%ebp), %eax
	movl	%eax, %ecx
	call	__ZZ4testvENKUlvE0_clEv
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE12:
	.ident	"GCC: (GNU) 4.8.1"
