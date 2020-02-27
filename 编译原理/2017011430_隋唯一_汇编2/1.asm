include Irvine32.inc

.data
MyArray  WORD 600h,500h,400h
Last WORD ?
.code
main PROC
   mov eax,OFFSET Last
   sub eax,OFFSET MyArray
   call WriteDec

    exit

main ENDP
END main