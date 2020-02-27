include Irvine32.inc

mWriteInt MACRO number
	mov eax,0
	IF TYPE number EQ 1
		mov al,number
	ELSE
		IF TYPE number EQ 2
			mov ax,number
		ELSE 
			mov eax,number
		ENDIF
	ENDIF
	call WriteInt
ENDM
.data
num1 BYTE 114
num2 WORD 514
num3 DWORD 1919
.code
main PROC
mWriteInt num1
call Crlf
mWriteInt num2
call Crlf
mWriteInt num3
call Crlf

    exit

main ENDP
END main
