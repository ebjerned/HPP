	.file	"loop_invariants.c"
# GNU C17 (Ubuntu 11.3.0-1ubuntu1~22.04) version 11.3.0 (x86_64-linux-gnu)
#	compiled by GNU C version 11.3.0, GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version isl-0.24-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -mtune=generic -march=x86-64 -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection -fcf-protection
	.text
	.section	.rodata
.LC1:
	.string	"slow"
.LC3:
	.string	"Done. sum = %15.3f\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	subq	$48, %rsp	#,
	movl	%edi, -36(%rbp)	# argc, argc
	movq	%rsi, -48(%rbp)	# argv, argv
# loop_invariants.c:8:   int N = 10000, i, j;
	movl	$10000, -20(%rbp)	#, N
# loop_invariants.c:10:   double * data = (double*) malloc(N*N*sizeof(double));
	movl	-20(%rbp), %eax	# N, tmp113
	imull	%eax, %eax	# tmp113, _1
	cltq
# loop_invariants.c:10:   double * data = (double*) malloc(N*N*sizeof(double));
	salq	$3, %rax	#, _3
	movq	%rax, %rdi	# _3,
	call	malloc@PLT	#
	movq	%rax, -8(%rbp)	# tmp114, data
# loop_invariants.c:11:   for(i = 0; i < N*N; i++)
	movl	$0, -28(%rbp)	#, i
# loop_invariants.c:11:   for(i = 0; i < N*N; i++)
	jmp	.L2	#
.L3:
# loop_invariants.c:12:     data[i] = 0;
	movl	-28(%rbp), %eax	# i, tmp115
	cltq
	leaq	0(,%rax,8), %rdx	#, _5
	movq	-8(%rbp), %rax	# data, tmp116
	addq	%rdx, %rax	# _5, _6
# loop_invariants.c:12:     data[i] = 0;
	pxor	%xmm0, %xmm0	# tmp117
	movsd	%xmm0, (%rax)	# tmp117, *_6
# loop_invariants.c:11:   for(i = 0; i < N*N; i++)
	addl	$1, -28(%rbp)	#, i
.L2:
# loop_invariants.c:11:   for(i = 0; i < N*N; i++)
	movl	-20(%rbp), %eax	# N, tmp118
	imull	%eax, %eax	# tmp118, _7
# loop_invariants.c:11:   for(i = 0; i < N*N; i++)
	cmpl	%eax, -28(%rbp)	# _7, i
	jl	.L3	#,
# loop_invariants.c:28:   printf("slow\n");
	leaq	.LC1(%rip), %rax	#, tmp119
	movq	%rax, %rdi	# tmp119,
	call	puts@PLT	#
# loop_invariants.c:29:   for(i = 0; i<N; i++) {
	movl	$0, -28(%rbp)	#, i
# loop_invariants.c:29:   for(i = 0; i<N; i++) {
	jmp	.L4	#
.L7:
# loop_invariants.c:30:     for(j = 0; j<N; j++)
	movl	$0, -24(%rbp)	#, j
# loop_invariants.c:30:     for(j = 0; j<N; j++)
	jmp	.L5	#
.L6:
# loop_invariants.c:31:       data[i*N + j] += i*N/7.7 + j;
	movl	-28(%rbp), %eax	# i, tmp120
	imull	-20(%rbp), %eax	# N, tmp120
	movl	%eax, %edx	# tmp120, _8
	movl	-24(%rbp), %eax	# j, tmp121
	addl	%edx, %eax	# _8, _9
	cltq
	leaq	0(,%rax,8), %rdx	#, _11
	movq	-8(%rbp), %rax	# data, tmp122
	addq	%rdx, %rax	# _11, _12
	movsd	(%rax), %xmm1	# *_12, _13
# loop_invariants.c:31:       data[i*N + j] += i*N/7.7 + j;
	movl	-28(%rbp), %eax	# i, tmp123
	imull	-20(%rbp), %eax	# N, _14
# loop_invariants.c:31:       data[i*N + j] += i*N/7.7 + j;
	pxor	%xmm0, %xmm0	# _15
	cvtsi2sdl	%eax, %xmm0	# _14, _15
	movsd	.LC2(%rip), %xmm3	#, tmp124
	movapd	%xmm0, %xmm2	# _15, _15
	divsd	%xmm3, %xmm2	# tmp124, _15
# loop_invariants.c:31:       data[i*N + j] += i*N/7.7 + j;
	pxor	%xmm0, %xmm0	# _17
	cvtsi2sdl	-24(%rbp), %xmm0	# j, _17
	addsd	%xmm2, %xmm0	# _16, _18
# loop_invariants.c:31:       data[i*N + j] += i*N/7.7 + j;
	movl	-28(%rbp), %eax	# i, tmp125
	imull	-20(%rbp), %eax	# N, tmp125
	movl	%eax, %edx	# tmp125, _19
	movl	-24(%rbp), %eax	# j, tmp126
	addl	%edx, %eax	# _19, _20
	cltq
	leaq	0(,%rax,8), %rdx	#, _22
	movq	-8(%rbp), %rax	# data, tmp127
	addq	%rdx, %rax	# _22, _23
	addsd	%xmm1, %xmm0	# _13, _24
	movsd	%xmm0, (%rax)	# _24, *_23
# loop_invariants.c:30:     for(j = 0; j<N; j++)
	addl	$1, -24(%rbp)	#, j
.L5:
# loop_invariants.c:30:     for(j = 0; j<N; j++)
	movl	-24(%rbp), %eax	# j, tmp128
	cmpl	-20(%rbp), %eax	# N, tmp128
	jl	.L6	#,
# loop_invariants.c:29:   for(i = 0; i<N; i++) {
	addl	$1, -28(%rbp)	#, i
.L4:
# loop_invariants.c:29:   for(i = 0; i<N; i++) {
	movl	-28(%rbp), %eax	# i, tmp129
	cmpl	-20(%rbp), %eax	# N, tmp129
	jl	.L7	#,
# loop_invariants.c:36:   sum = 0;
	pxor	%xmm0, %xmm0	# tmp130
	movsd	%xmm0, -16(%rbp)	# tmp130, sum
# loop_invariants.c:37:   for(i = 0; i < N*N; i++)
	movl	$0, -28(%rbp)	#, i
# loop_invariants.c:37:   for(i = 0; i < N*N; i++)
	jmp	.L8	#
.L9:
# loop_invariants.c:38:     sum += data[i];
	movl	-28(%rbp), %eax	# i, tmp131
	cltq
	leaq	0(,%rax,8), %rdx	#, _26
	movq	-8(%rbp), %rax	# data, tmp132
	addq	%rdx, %rax	# _26, _27
	movsd	(%rax), %xmm0	# *_27, _28
# loop_invariants.c:38:     sum += data[i];
	movsd	-16(%rbp), %xmm1	# sum, tmp134
	addsd	%xmm1, %xmm0	# tmp134, tmp133
	movsd	%xmm0, -16(%rbp)	# tmp133, sum
# loop_invariants.c:37:   for(i = 0; i < N*N; i++)
	addl	$1, -28(%rbp)	#, i
.L8:
# loop_invariants.c:37:   for(i = 0; i < N*N; i++)
	movl	-20(%rbp), %eax	# N, tmp135
	imull	%eax, %eax	# tmp135, _29
# loop_invariants.c:37:   for(i = 0; i < N*N; i++)
	cmpl	%eax, -28(%rbp)	# _29, i
	jl	.L9	#,
# loop_invariants.c:39:   printf("Done. sum = %15.3f\n", sum);
	movq	-16(%rbp), %rax	# sum, tmp136
	movq	%rax, %xmm0	# tmp136,
	leaq	.LC3(%rip), %rax	#, tmp137
	movq	%rax, %rdi	# tmp137,
	movl	$1, %eax	#,
	call	printf@PLT	#
# loop_invariants.c:41:   free(data);
	movq	-8(%rbp), %rax	# data, tmp138
	movq	%rax, %rdi	# tmp138,
	call	free@PLT	#
# loop_invariants.c:43:   return 0;
	movl	$0, %eax	#, _49
# loop_invariants.c:44: }
	leave	
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC2:
	.long	-858993459
	.long	1075760332
	.ident	"GCC: (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0"
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
