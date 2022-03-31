.global LoadGdt
LoadGdt:
	ldgt [rdi]
	mov ax, 0x40
	ltr ax
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	popq rdi
	mov rax, 0x08
	pushq rax
	pushq rdi
	lretq
