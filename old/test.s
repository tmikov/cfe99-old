	.file	"test.c"
gcc2_compiled.:
___gnu_compiled_c:
.text
	.align 4
.globl _func1
	.def	_func1;	.scl	2;	.type	32;	.endef
_func1:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	movl $0,-4(%ebp)
	incl -4(%ebp)
	movl -4(%ebp),%edx
	movl %edx,%eax
	jmp L2
	.align 4
L2:
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
.globl _func2
	.def	_func2;	.scl	2;	.type	32;	.endef
_func2:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	movl %esp,-8(%ebp)
	nop
L4:
	jmp L4
	nop
L5:
	jmp L5
	.align 4
L3:
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
.globl _func3
	.def	_func3;	.scl	2;	.type	32;	.endef
_func3:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	movl $L7+4,-4(%ebp)
L7:
	movl -4(%ebp),%eax
	addl $4,%eax
	jmp *%eax
	.align 4
L6:
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
.globl _func4
	.def	_func4;	.scl	2;	.type	32;	.endef
_func4:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
L8:
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
.globl _func5
	.def	_func5;	.scl	2;	.type	32;	.endef
_func5:
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%edx
	movl %edx,%eax
	testl %edx,%edx
	jne L10
	movl 12(%ebp),%eax
L10:
	movl %eax,%eax
	jmp L9
L9:
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
.globl _func6
	.def	_func6;	.scl	2;	.type	32;	.endef
_func6:
	pushl %ebp
	movl %esp,%ebp
L11:
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
.globl _func7
	.def	_func7;	.scl	2;	.type	32;	.endef
_func7:
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	movl %esp,-8(%ebp)
L12:
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
LC0:
	.long 1
	.long 1
	.long 1
	.long 1
	.long 1
	.long 1
	.long 10
	.long 10
	.long 10
	.long 10
	.long 10
	.align 4
.globl _func8
	.def	_func8;	.scl	2;	.type	32;	.endef
_func8:
	pushl %ebp
	movl %esp,%ebp
	subl $64,%esp
	pushl %edi
	pushl %esi
	leal -48(%ebp),%eax
	leal -48(%ebp),%edi
	movl $LC0,%esi
	cld
	movl $11,%ecx
	rep
	movsl
L15:
	leal -72(%ebp),%esp
	popl %esi
	popl %edi
	movl %ebp,%esp
	popl %ebp
	ret
	.align 4
.globl _func10
	.def	_func10;	.scl	2;	.type	32;	.endef
_func10:
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%eax
	cmpl $5,%eax
	jg L19
	cmpl $1,%eax
	jl L19
	jmp L18
	.align 4
L18:
	movl $1,%eax
	jmp L16
	.align 4
L19:
	xorl %eax,%eax
	jmp L16
	.align 4
L17:
L16:
	movl %ebp,%esp
	popl %ebp
	ret
