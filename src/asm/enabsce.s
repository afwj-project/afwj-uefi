.intel_syntax noprefix
EnableSce:
	mov rcx, 0xC0000080
	rdmsr
	or eax, 1
	wrmsr
	mov rcx, 0xC0000081
	rdmsr
	mov edx, 0x00180008
	wrmsr
	ret
