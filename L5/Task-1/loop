	.file	"loop_invariants.c"
# GNU C17 (Ubuntu 11.3.0-1ubuntu1~22.04) version 11.3.0 (x86_64-linux-gnu)
#	compiled by GNU C version 11.3.0, GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version isl-0.24-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -mtune=generic -march=x86-64 -O3 -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection -fcf-protection
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC2:
	.string	"slow"
.LC5:
	.string	"Done. sum = %15.3f\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB39:
	.cfi_startproc
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
# loop_invariants.c:10:   double * data = (double*) malloc(N*N*sizeof(double));
	movl	$1, %esi	#,
	movl	$800000000, %edi	#,
	call	calloc@PLT	#
# /usr/include/x86_64-linux-gnu/bits/stdio2.h:112:   return __printf_chk (__USE_FORTIFY_LEVEL - 1, __fmt, __va_arg_pack ());
	leaq	.LC2(%rip), %rdi	#, tmp114
# loop_invariants.c:10:   double * data = (double*) malloc(N*N*sizeof(double));
	movq	%rax, %rbp	# tmp134, data
# /usr/include/x86_64-linux-gnu/bits/stdio2.h:112:   return __printf_chk (__USE_FORTIFY_LEVEL - 1, __fmt, __va_arg_pack ());
	call	puts@PLT	#
	movq	%rbp, %rsi	# data, ivtmp.22
	movq	%rbp, %rdx	# data, ivtmp.43
	xorl	%ecx, %ecx	# ivtmp.42
	movdqa	.LC3(%rip), %xmm4	#, tmp132
	movsd	.LC4(%rip), %xmm6	#, tmp133
	movdqa	.LC0(%rip), %xmm5	#, vect_vec_iv_.8
.L2:
# loop_invariants.c:31:       data[i*N + j] += i*N/7.7 + j;
	pxor	%xmm3, %xmm3	# tmp124
	xorl	%eax, %eax	# ivtmp.28
	movdqa	%xmm5, %xmm2	# vect_vec_iv_.8, vect_vec_iv_.8
	cvtsi2sdl	%ecx, %xmm3	# ivtmp.42, tmp124
	divsd	%xmm6, %xmm3	# tmp133, _12
	unpcklpd	%xmm3, %xmm3	# vect_cst__42
	.p2align 4,,10
	.p2align 3
.L3:
	movdqa	%xmm2, %xmm0	# vect_vec_iv_.8, vect_vec_iv_.8
# loop_invariants.c:31:       data[i*N + j] += i*N/7.7 + j;
	movupd	16(%rdx,%rax), %xmm7	# MEM <vector(2) double> [(double *)vectp.10_52 + 16B + ivtmp.28_9 * 1], tmp136
	paddd	%xmm4, %xmm2	# tmp132, vect_vec_iv_.8
# loop_invariants.c:31:       data[i*N + j] += i*N/7.7 + j;
	pshufd	$238, %xmm0, %xmm1	#, vect_vec_iv_.8, tmp117
	cvtdq2pd	%xmm0, %xmm0	# vect_vec_iv_.8, vect__13.13
	addpd	%xmm3, %xmm0	# vect_cst__42, vect__14.14
	cvtdq2pd	%xmm1, %xmm1	# tmp117, vect__13.13
	addpd	%xmm3, %xmm1	# vect_cst__42, vect__14.14
# loop_invariants.c:31:       data[i*N + j] += i*N/7.7 + j;
	addpd	%xmm7, %xmm1	# tmp136, vect__15.15
	movupd	(%rdx,%rax), %xmm7	# MEM <vector(2) double> [(double *)vectp.10_52 + ivtmp.28_9 * 1], tmp137
	addpd	%xmm7, %xmm0	# tmp137, vect__15.15
	movups	%xmm1, 16(%rdx,%rax)	# vect__15.15, MEM <vector(2) double> [(double *)vectp.10_52 + 16B + ivtmp.28_9 * 1]
	movups	%xmm0, (%rdx,%rax)	# vect__15.15, MEM <vector(2) double> [(double *)vectp.10_52 + ivtmp.28_9 * 1]
	addq	$32, %rax	#, ivtmp.28
	cmpq	$80000, %rax	#, ivtmp.28
	jne	.L3	#,
# loop_invariants.c:29:   for(i = 0; i<N; i++) {
	addl	$10000, %ecx	#, ivtmp.42
	addq	$80000, %rdx	#, ivtmp.43
	cmpl	$100000000, %ecx	#, ivtmp.42
	jne	.L2	#,
	leaq	800000000(%rbp), %rax	#, _8
# loop_invariants.c:36:   sum = 0;
	pxor	%xmm0, %xmm0	# sum
.L5:
	movsd	(%rsi), %xmm1	# MEM <vector(2) double> [(double *)_6], stmp_sum_32.7
	addq	$16, %rsi	#, ivtmp.22
	addsd	%xmm1, %xmm0	# stmp_sum_32.7, stmp_sum_32.7
# loop_invariants.c:38:     sum += data[i];
	movsd	-8(%rsi), %xmm1	# MEM <vector(2) double> [(double *)_6], stmp_sum_32.7
	addsd	%xmm1, %xmm0	# stmp_sum_32.7, sum
	cmpq	%rsi, %rax	# ivtmp.22, _8
	jne	.L5	#,
# /usr/include/x86_64-linux-gnu/bits/stdio2.h:112:   return __printf_chk (__USE_FORTIFY_LEVEL - 1, __fmt, __va_arg_pack ());
	leaq	.LC5(%rip), %rsi	#, tmp129
	movl	$1, %edi	#,
	movl	$1, %eax	#,
	call	__printf_chk@PLT	#
# loop_invariants.c:41:   free(data);
	movq	%rbp, %rdi	# data,
	call	free@PLT	#
# loop_invariants.c:44: }
	xorl	%eax, %eax	#
	popq	%rbp	#
	.cfi_def_cfa_offset 8
	ret	
	.cfi_endproc
.LFE39:
	.size	main, .-main
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC0:
	.long	0
	.long	1
	.long	2
	.long	3
	.align 16
.LC3:
	.long	4
	.long	4
	.long	4
	.long	4
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC4:
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
