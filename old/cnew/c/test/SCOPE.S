	.file	"SCOPE.cc"
gcc2_compiled.:
___gnu_compiled_cplusplus:
.text
	.align 2
.globl _func1__Fi
_func1__Fi:
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%edx
	decl %edx
	movl %edx,%eax
	jmp L1
	.align 2,0x90
	jmp L2
	.align 2,0x90
	jmp L1
	.align 2,0x90
L2:
L1:
	leave
	ret
	.align 2
.globl _func2__FP1S
_func2__FP1S:
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%eax
	movl (%eax),%edx
	decl %edx
	movl %edx,%eax
	jmp L3
	.align 2,0x90
	jmp L4
	.align 2,0x90
	jmp L3
	.align 2,0x90
L4:
L3:
	leave
	ret
	.align 2
.globl _func3__Fv
_func3__Fv:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl $0,-4(%ebp)
	movl -4(%ebp),%edx
	decl %edx
	movl %edx,%eax
	jmp L5
	.align 2,0x90
	jmp L6
	.align 2,0x90
	jmp L5
	.align 2,0x90
L6:
L5:
	leave
	ret
	.align 2
.globl _func4__Fv
_func4__Fv:
	pushl %ebp
	movl %esp,%ebp
L8:
	jmp L8
	.align 2,0x90
	jmp L9
	.align 2,0x90
	jmp L7
	.align 2,0x90
L9:
L7:
	leave
	ret
