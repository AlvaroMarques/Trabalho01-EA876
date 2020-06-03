start: ; exemplo de uso
	mov A, 5
	mov B, 3
	call exp
	hlt

exp: ; recebe A, B e retorna o valor de A**B no registrador A
	push C
	push D
	mov C, B
	mov B, A
	mov D, 0
	mov A, 1
	cmp D,C
	JE end
	for:
	inc D
	mul B
	cmp D,C
	JE end
	JMP for

	end:
	pop D
	pop C
	RET

