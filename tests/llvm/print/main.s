	.text
	.file	"main.lua"
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3                               # -- Begin function main
.LCPI0_0:
	.quad	0x4028000000000000              # double 12
	.text
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	movsd	.LCPI0_0(%rip), %xmm0           # xmm0 = mem[0],zero
	movl	$.L__unnamed_1, %edi
	movl	$.L__unnamed_2, %esi
	movb	$1, %al
	callq	printf@PLT
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	.L__unnamed_1,@object           # @0
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_1:
	.asciz	"Hello, %s! %f\n"
	.size	.L__unnamed_1, 15

	.type	.L__unnamed_2,@object           # @1
.L__unnamed_2:
	.asciz	"world"
	.size	.L__unnamed_2, 6

	.section	".note.GNU-stack","",@progbits
