include Irvine32.inc

.data
emp BYTE " ",0
.code

GCD PROC
    test eax,80000000h
	jz ok1
	neg eax             ;abs(x)
	ok1: test ebx,80000000h
	jz ok2
	neg ebx             ;abs(y)
	ok2:
	    L:
		    mov edx,0
			div ebx
			mov eax,ebx
			mov ebx,edx
			test ebx,0FFFFFFFFh
			jnz L
    ret
GCD ENDP


main PROC
    mov eax,15
	mov ebx,20
	call GCD
	call WriteDec
	mov edx,OFFSET emp
	call WriteString
	mov eax,-15
	mov ebx,120
	call GCD
	call WriteDec
	mov edx,OFFSET emp
	call WriteString
	mov eax,114
	mov ebx,514
	call GCD
	call WriteDec
    exit

main ENDP
END main