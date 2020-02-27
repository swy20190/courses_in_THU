include Irvine32.inc

.data
he BYTE "Stack parameters:",0Dh,0Ah,0
line1 BYTE "---------------------------",0Dh,0Ah
newline BYTE 0Dh,0Ah,0
address BYTE "ADDRESS ",0
eq1 BYTE " = ",0
.code

ShowParams PROC,
	number:DWORD
	mov esi,ebp
	mov ecx,number
	mov edx,OFFSET he
	call WriteString
	mov edx,OFFSET line1
	call WriteString
	mov eax,number
	add eax,2
	mov ebx,4
	mul ebx
	add esi,eax
	L:
		mov edx,OFFSET address
		call WriteString
		mov eax,esi
		call WriteHex
		mov edx,OFFSET eq1
		call WriteString
		mov eax,dword ptr [esi]
		call WriteHex
		mov edx,OFFSET newline
		call WriteString
		add esi,4
		loop L
	ret
ShowParams ENDP

MySample PROC,
	first:DWORD,
	second:DWORD,
	third:DWORD
	paramCount = 3
	INVOKE ShowParams,paramCount
	ret
MySample ENDP

main PROC
    INVOKE MySample, 1234h, 5000h, 6543h
    exit

main ENDP
END main