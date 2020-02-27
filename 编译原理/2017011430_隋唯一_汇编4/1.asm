include Irvine32.inc
BUFFER_SIZE = 514

.data
count DWORD 10
buffer BYTE BUFFER_SIZE DUP(?)
filename BYTE "info.txt",0
fileHandle HANDLE ?
stringLength DWORD ?
byteswritten DWORD ?
str1 BYTE "Cannot create file",0dh,0ah,0
str2 BYTE "Bytes written to file [info.txt]:",0
str3 BYTE "Enter up to 514 chars and press"
     BYTE "[ENTER]:",0ah,0dh,0

.code
main PROC
	mov edx,offset filename
	call CreateOutputFile
	mov fileHandle,eax

	cmp eax,INVALID_HANDLE_VALUE
	jne file_ok
	mov edx,offset str1
	call WriteString
	jmp quit

file_ok:
	REPEAT 10
		mov edx,offset str3
		call WriteString
		mov ecx,BUFFER_SIZE
		mov edx,offset buffer
		call ReadString
		mov stringLength,eax

		mov eax,fileHandle
		mov edx,offset buffer
		mov ecx,stringLength
		call WriteToFile
		INVOKE SetFilePointer,
			filehandle,
			0,
			0,
			FILE_END
	ENDM
	
	
	
	call CloseFile

	mov edx,offset str2
	call WriteString
	call Crlf

quit:
	exit
main ENDP
END main
