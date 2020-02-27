include Irvine32.inc

.data
intArray  WORD 500h, 400h, 300h, 200h, 100h

.code
main PROC
   mov ecx,5
   mov eax,0
   mov esi,OFFSET intArray
   L:
       add ax,[esi]
	   add esi,2
	   loop L
   call WriteDec
    exit

main ENDP
END main