.intel_syntax noprefix
.global ToUserspace
ToUserspace:
	mov rcx, rdi
	mov rsp, rsi
	mov r11, 0x0202
	sysretq
