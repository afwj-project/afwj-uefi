.global LoadPml4
LoadPml4:
	mov rax, 0x000FFFFFFFFFF000
	and rdi, rax
	mov cr3, rdi
	ret
