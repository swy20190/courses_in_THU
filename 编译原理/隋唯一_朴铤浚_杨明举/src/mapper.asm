include mapper.inc


.code

mapper_init proc uses ebx ecx edx esi 
	invoke crt_malloc, (size mapper) * 256
	mov ebx, eax
	mov ecx, 256
	mov esi, 0
L1:
	mov eax, size mapper
	mul esi
	mov edx, ebx
	add edx, eax
	mov (mapper ptr [edx]).bit_length, 0
	mov (mapper ptr [edx]).weight, 0
	inc esi
	loop L1
	mov eax, ebx
	ret
mapper_init endp



mapper_destroy proc, mappers: ptr mapper
	mov eax, mappers
	cmp eax, 0
	je return
	invoke crt_free, mappers
return:
	ret
mapper_destroy endp

mapper_set proc uses ecx ebx esi eax, mappers: ptr mapper, order: byte, 
	bit_length: sdword, bits: ptr byte, weight: sdword
	mov esi, mappers
	mov eax, size mapper
	movzx ebx, order
	mul ebx
	add esi, eax
	mov ebx, bit_length
	mov (mapper ptr [esi]).bit_length, ebx
	mov ebx, weight
	mov (mapper ptr [esi]).weight, ebx
	mov ecx, bit_length
	mov eax, bits
L1: 
	mov bl, byte ptr [eax + ecx - 1]
	mov byte ptr [esi + ecx + 3], bl
	loop L1

	ret
mapper_set endp

_mapper_set_all proc, mappers: ptr mapper, forest: ptr huffman_node, depth: sdword, bits: ptr byte
	pushad
	mov eax, forest
	cmp  dword ptr [eax + 5], 0
	jne lc
	mov eax, forest
	; (!(forest->left_child) && !(forest->right_child))
	cmp  dword ptr [eax + 9],0
	jne lc

	; mapper_set(mappers, forest->key, depth + 1, bits, forest->weight)
	mov eax, forest
	mov ecx, [eax + 1]
	push ecx
	mov edx, bits
	push edx
	mov eax, depth
	add eax, 1
	push eax
	mov ecx, forest
	movzx edx,  byte ptr [ecx]
	push edx
	mov eax, mappers
	push eax
	call mapper_set
	add esp, 14h
	jmp return
lc:
	; if (forest->left_child)
	mov eax, forest
	cmp  dword ptr [eax + 5], 0
	je rc
	; bits[depth + 1] = 0;
	mov eax, bits
	add eax, depth
	mov  byte ptr [eax+1], 0

	; _mapper_set_all(mappers, forest->left_child, depth + 1, bits)
	mov eax, bits
	push eax
	mov ecx, depth
	add ecx, 1
	push ecx
	mov edx, forest
	mov eax, [edx + 5]
	push eax
	mov ecx, mappers
	push ecx
	call _mapper_set_all
	add esp, 10h
rc:
	; if (forest->right_child)
	mov eax, forest
	cmp  dword ptr [eax + 9], 0
	je return
	; bits[depth + 1] = 1
	mov eax, bits
	add eax, depth
	mov  byte ptr [eax + 1], 1

	; _mapper_set_all(mappers, forest->right_child, depth + 1, bits)
	mov eax, bits
	push eax
	mov ecx, depth
	add ecx, 1
	push ecx
	mov edx, forest
	mov eax, [edx + 9]
	push eax
	mov ecx, mappers
	push ecx
	call _mapper_set_all
	add esp, 10h

return:
	popad
	ret
_mapper_set_all endp


mapper_set_all proc uses ecx, mappers: ptr mapper, forest: ptr huffman_node
	local bits[256]: byte 
	mov ecx, 256
L1: 
	mov bits[ecx - 1], 0
	loop L1
	invoke _mapper_set_all, mappers, forest, 0, addr bits
	ret
mapper_set_all endp

end
