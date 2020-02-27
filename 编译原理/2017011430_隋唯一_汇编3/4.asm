include Irvine32.inc

.data
myArray WORD 3, 1, 7, 5, 2, 9, 4, 3
myArray2 WORD 1,1,4,5,1,4
.code
flag equ DWORD PTR [ebp-4]
count equ DWORD PTR [ebp-8]
BubbleSort PROC,
	array:PTR WORD,
	leng:DWORD
	mov flag,0
	mov ecx,leng
	sub ecx,1
  L1:
	mov esi,array
	mov flag,0
    mov count,ecx
	mov ecx,count
  L2:
	mov ax,[esi]
	mov bx,[esi+2]
    cmp ax,bx
	JC endL2
	mov ax,[esi]
	mov bx,[esi+2]
	mov [esi+2],ax
	mov [esi],bx
	mov flag,1
  endL2:
   add esi,2
   loop L2
   mov ecx,count
   test flag,1
   loopnz L1
   ret
BubbleSort ENDP
    

main PROC
	INVOKE BubbleSort,ADDR myArray,8
	mov eax,0
	mov ecx,8
	mov esi,OFFSET myArray
	L1:
	mov ax,[esi]
	call WriteDec
	add esi,2
	loop L1
	INVOKE BubbleSort,ADDR myArray2,6
	mov eax,0
	mov ecx,6
	mov esi,OFFSET myArray2
	L2:
	mov ax,[esi]
	call WriteDec
	add esi,2
	loop L2
    exit

main ENDP
END main