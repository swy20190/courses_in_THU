include Irvine32.inc

.data
emp BYTE " ",0

.code
main PROC
    mov eax,0;g
	mov ebx,1;f
	mov ecx,12
	L:
	    add eax,ebx;g+=f
		mov edx,eax
		sub edx,ebx
		mov ebx,edx;f=g-f
		call WriteDec
		mov edx,OFFSET emp
		call WriteString;输出空格
		loop L
    exit

main ENDP
END main