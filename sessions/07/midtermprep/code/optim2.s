	.file	"optim2.cpp"
	.text
	.p2align 4
	.globl	_Z3sumj
	.type	_Z3sumj, @function
_Z3sumj:
.LFB1812:
	.cfi_startproc
	endbr64
	testl	%edi, %edi
	je	.L8
	leal	-1(%rdi), %eax
	cmpl	$18, %eax
	jbe	.L9
	cmpl	$-1, %edi
	je	.L9
	movl	%edi, %edx
	movdqa	.LC0(%rip), %xmm2
	xorl	%eax, %eax
	pxor	%xmm1, %xmm1
	movdqa	.LC1(%rip), %xmm5
	shrl	$2, %edx
	pxor	%xmm3, %xmm3
	.p2align 4,,10
	.p2align 3
.L4:
	movdqa	%xmm2, %xmm0
	addl	$1, %eax
	paddd	%xmm5, %xmm2
	movdqa	%xmm0, %xmm4
	punpckhdq	%xmm3, %xmm0
	punpckldq	%xmm3, %xmm4
	paddq	%xmm4, %xmm0
	paddq	%xmm0, %xmm1
	cmpl	%edx, %eax
	jb	.L4
	movdqa	%xmm1, %xmm0
	movl	%edi, %edx
	psrldq	$8, %xmm0
	andl	$-4, %edx
	paddq	%xmm0, %xmm1
	leal	1(%rdx), %ecx
	movq	%xmm1, %rax
	cmpl	%edx, %edi
	je	.L1
	addq	%rcx, %rax
	leal	2(%rdx), %ecx
	cmpl	%ecx, %edi
	jb	.L1
	addq	%rcx, %rax
	leal	3(%rdx), %ecx
	movq	%rcx, %rdx
	addq	%rax, %rcx
	cmpl	%edx, %edi
	cmovnb	%rcx, %rax
	ret
	.p2align 4,,10
	.p2align 3
.L8:
	xorl	%eax, %eax
.L1:
	ret
	.p2align 4,,10
	.p2align 3
.L9:
	movl	$1, %edx
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L3:
	movl	%edx, %ecx
	addl	$1, %edx
	addq	%rcx, %rax
	cmpl	%edx, %edi
	jnb	.L3
	ret
	.cfi_endproc
.LFE1812:
	.size	_Z3sumj, .-_Z3sumj
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC2:
	.string	"sum="
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB1813:
	.cfi_startproc
	endbr64
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movl	$4, %edx
	leaq	.LC2(%rip), %rsi
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	leaq	_ZSt4cout(%rip), %rbp
	movq	%rbp, %rdi
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
	movq	%rbp, %rdi
	movabsq	$499999999500000000, %rsi
	call	_ZNSo9_M_insertIxEERSoT_@PLT
	movq	%rax, %rbp
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%rbp,%rax), %r12
	testq	%r12, %r12
	je	.L24
	cmpb	$0, 56(%r12)
	je	.L21
	movzbl	67(%r12), %eax
.L22:
	movq	%rbp, %rdi
	movsbl	%al, %esi
	call	_ZNSo3putEc@PLT
	movq	%rax, %rdi
	call	_ZNSo5flushEv@PLT
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L21:
	.cfi_restore_state
	movq	%r12, %rdi
	call	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT
	movq	(%r12), %rax
	movl	$10, %esi
	movq	%r12, %rdi
	call	*48(%rax)
	jmp	.L22
.L24:
	call	_ZSt16__throw_bad_castv@PLT
	.cfi_endproc
.LFE1813:
	.size	main, .-main
	.p2align 4
	.type	_GLOBAL__sub_I__Z3sumj, @function
_GLOBAL__sub_I__Z3sumj:
.LFB2304:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	leaq	_ZStL8__ioinit(%rip), %rbp
	movq	%rbp, %rdi
	call	_ZNSt8ios_base4InitC1Ev@PLT
	movq	_ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rdi
	movq	%rbp, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 8
	leaq	__dso_handle(%rip), %rdx
	jmp	__cxa_atexit@PLT
	.cfi_endproc
.LFE2304:
	.size	_GLOBAL__sub_I__Z3sumj, .-_GLOBAL__sub_I__Z3sumj
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I__Z3sumj
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC0:
	.long	1
	.long	2
	.long	3
	.long	4
	.align 16
.LC1:
	.long	4
	.long	4
	.long	4
	.long	4
	.hidden	__dso_handle
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04.2) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
