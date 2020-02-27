include Irvine32.inc

.data
targetStr BYTE "abcde",10 DUP(0)
sourceStr BYTE "fgh",0
newline BYTE 0Dh,0Ah,0
.code
Str_concat PROC,
	address1:PTR BYTE,
	address2:PTR BYTE
	mov eax,0
	mov esi,0
	mov esi,address1
	mov al,[esi]
	test eax,0FFFFFFFFh
	jz next
	L1:
		inc esi
		mov al,[esi]
		test eax,0FFFFFFFFh
		jnz L1
	next:
	mov edi,0
	mov edi,address2
	L2:
		mov eax,0
		mov al,[edi]
		test eax,0FFFFFFFFh
		jz over
		mov [esi],al
		inc esi
		inc edi
		jmp L2
	over:
		ret
Str_concat ENDP

main PROC
    mov edx,OFFSET targetStr
	call WriteString
	mov edx,OFFSET newline
	call WriteString
	mov edx,OFFSET sourceStr
	call WriteString
	mov edx,OFFSET newline
	call WriteString
	INVOKE Str_concat,ADDR targetStr,ADDR sourceStr
	mov edx,OFFSET targetStr
	call WriteString
    exit

main ENDP
END main