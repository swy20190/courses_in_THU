;--------------------------
;this file is settings of the md5 algorithm
;define the F,G,H,I functions
;created by swy 2019/10/17
;---------------------------




.model flat, c

include huffman_forest.inc
include msvcrt.inc
includelib msvcrt.lib

S11 = 7
S12 = 12
S13 = 17
S14 = 22
S21 = 5
S22 = 9
S23 = 14
S24 = 20
S31 = 4
S32 = 11
S33 = 16
S34 = 23
S41 = 6
S42 = 10
S43 = 15
S44 = 21

FF macro a,b,c,d,k,s,i
	mov	edi,b
	mov	ebp,b
	and	edi,c
	not	ebp
	and	ebp,d
	or	edi,ebp
	lea	a,dword ptr [a+edi+i]
	add	a,dword ptr [esi+k*4]
	rol	a,s
	add	a,b
endm FF

GG macro a,b,c,d,k,s,i
	mov	edi,d
	mov	ebp,d
	and	edi,b
	not	ebp
	and	ebp,c
	or	edi,ebp
	lea	a,dword ptr [a+edi+i]
	add	a,dword ptr [esi+k*4]
	rol	a,s
	add	a,b
endm GG

HH macro a,b,c,d,k,s,i
	mov	ebp,b
	xor	ebp,c
	xor	ebp,d
	lea	a,dword ptr [a+ebp+i]
	add	a,dword ptr [esi+k*4]
	rol	a,s
	add	a,b
endm HH

II macro a,b,c,d,k,s,i
	mov	ebp,d
	not	ebp
	or	ebp,b
	xor	ebp,c
	lea	a,dword ptr [a+ebp+i]
	add	a,dword ptr [esi+k*4]
	rol	a,s
	add	a,b
endm II

md5 proto, password: ptr byte

password_is_valid proto, input_password: ptr byte, info_buffer: ptr huffman_buffer

calc_md5 proto, block_size: sdword, datablock: ptr byte, dest_hash: ptr byte
