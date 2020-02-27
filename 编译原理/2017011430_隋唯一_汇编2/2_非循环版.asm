include Irvine32.inc

.data
intArray  WORD 500h, 400h, 300h, 200h, 100h

.code
main PROC
   mov esi,OFFSET intArray
   mov eax,0
   mov ax,[esi]
   add esi,2
   add ax,[esi]
   add esi,2
   add ax,[esi]
   add esi,2
   add ax,[esi]
   add esi,2
   add ax,[esi]
   call WriteDec

    exit

main ENDP
END main