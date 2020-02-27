include Irvine32.inc

.data
source BYTE 'This is the source string', 0
target BYTE SIZEOF source DUP(0)

.code
main PROC
   mov esi,0
   mov ecx,SIZEOF source
   L:
      mov al,source[esi]
	  mov target[esi],al
	  inc esi
	  loop L 
   mov edx,OFFSET target
   call WriteString
    exit

main ENDP
END main