;------------------------------------------
;this src file named b_buffer is created to 
;load the input file
;created by swy 2019/10/14
;------------------------------------------



include huffman_buffer.inc

.data
rb_mode byte "rb", 0
wb_mode byte "wb", 0

.code
huffman_buffer_create proc USES ebx ecx esi, max_byte_count: sdword
	local buffer: dword
	mov ebx, max_byte_count
	pushad
	invoke crt_malloc, SIZE huffman_buffer
	mov (huffman_buffer ptr [eax]).current_byte_size, 0
	mov (huffman_buffer ptr [eax]).byte_capacity, ebx
	mov buffer, eax
	popad

	invoke crt_malloc, max_byte_count
	mov ebx, buffer
	mov (huffman_buffer ptr [ebx]).buffer, eax
	.if max_byte_count == 0
		jmp quit
	.endif
	mov esi, eax
	mov ecx, max_byte_count
L1:
	; mov byte ptr (huffman_buffer ptr [ebx]).buffer[ecx - 1], 0
	mov byte ptr [esi + ecx - 1], 0
	loop L1

quit:
	mov eax, buffer
	ret
huffman_buffer_create endp


huffman_buffer_destroy proc uses esi, buffer: ptr huffman_buffer
	.if buffer != 0
		mov esi, buffer
		mov esi, (huffman_buffer ptr [esi]).buffer
		.if esi != 0
			invoke crt_free, esi
		.endif
		mov esi, buffer
		invoke crt_free, esi
	.endif
	ret
huffman_buffer_destroy endp


huffman_buffer_insert proc uses ebx ecx esi, buffer: ptr huffman_buffer, bit_count: sdword, bits: ptr byte

	mov ecx, bit_count
L1:
	mov esi, bits
	mov eax, bit_count
	sub eax, ecx
	
	mov bl, byte ptr [esi + eax]
	.if bl == 1
		mov esi, buffer
		mov eax, (huffman_buffer ptr [esi]).current_byte_size
		push ecx
		mov ecx, eax
		
		sar eax, 3

		and ecx, 00000007h
		.if ecx != 0
			mov ebx, 1
L2:	
			sal ebx, 1
			loop L2
		.endif

		mov esi, (huffman_buffer ptr [esi]).buffer
		or byte ptr [esi + eax], bl
		pop ecx
	.endif
	mov esi, buffer
	inc (huffman_buffer ptr [esi]).current_byte_size
	loop L1
	ret
huffman_buffer_insert endp


get_compressed_size proc uses ebx esi, buffer: ptr huffman_buffer
	mov esi, buffer
	mov eax, sdword ptr [esi]
	sar eax, 3

	mov ebx, sdword ptr [esi]
	and ebx, 00000007h

	.IF ebx != 0
		inc eax
	.ENDIF
	ret	
get_compressed_size endp


compress_into_buffer proc uses esi edi, file_name: ptr byte, buffer: ptr huffman_buffer, mappers: ptr mapper
	local file_stream: dword
	local char: sdword
	
	mov file_stream, 0
	pushad
	invoke crt_fopen, file_name, offset rb_mode
	.if	eax == 0
		ret
	.endif
	mov file_stream, eax
	popad

L1:
	pushad
	invoke crt_fgetc, file_stream
	mov char, eax
	popad
	.if char == -1
		jmp quit
	.endif

	mov esi, SIZE mapper
	imul esi, char
	add esi, mappers
	mov edi, esi
	add esi, 4
	invoke huffman_buffer_insert, buffer, (mapper ptr [edi]).bit_length, esi
	jmp L1

quit:
	invoke crt_fclose, file_stream
	ret
compress_into_buffer endp


save_encode_info_into_buffer proc uses ebx ecx edx esi, buffer: ptr huffman_buffer, decompressed_size: sdword, compressed_size: sdword, 
															password: ptr byte, mappers: ptr mapper
	local password_offset: ptr byte
	local data_offset: ptr byte
	
	mov eax, buffer
	mov eax, (huffman_buffer ptr [eax]).buffer
	mov ebx, decompressed_size
	mov sdword ptr [eax], ebx
	add eax, 4
	mov ebx, compressed_size
	mov sdword ptr [eax], ebx

	add eax, 4
	mov password_offset, eax

 	mov ecx, 16
	mov esi, password
	mov edi, password_offset
L1:
	mov eax, 16
	sub eax, ecx
	mov bl, byte ptr [esi + eax]
	mov byte ptr [edi + eax], bl
	loop L1

	mov data_offset, edi
	add data_offset, 16

	mov ecx, 256
	mov esi, mappers
L2:

	mov eax, 256
	sub eax, ecx
	mov ebx, eax
	mov edx, SIZE mapper
	mul edx
	; add esi, eax
	mov eax, (mapper ptr [esi + eax]).weight
	imul ebx, 4
	add ebx, data_offset
	mov sdword ptr [ebx], eax
	loop L2

	mov ebx, DECODE_INFO_BUFFER_SIZE
	sal ebx, 3
	mov eax, buffer
	add (huffman_buffer ptr [eax]).current_byte_size, ebx
	ret
save_encode_info_into_buffer endp


write_into_file proc uses ebx ecx esi, info_buffer: ptr huffman_buffer, data_buffer: ptr huffman_buffer, file_name: ptr byte
	local file_stream: dword
	local count: sdword
	
	mov file_stream, 0
	pushad
	invoke crt_fopen, file_name, offset wb_mode
	mov file_stream, eax
	popad
	.if file_stream == 0
		ret
	.endif

	.if info_buffer != 0
		mov eax, info_buffer
		mov eax, (huffman_buffer ptr [eax]).current_byte_size
		mov ebx, eax
		sar eax, 3
		and ebx, 00000007h
		.if ebx != 0
			inc eax
		.endif

		mov count, eax
		.if count == 0
			jmp quit
		.endif
		mov ecx, count
L1:
		mov eax, count
		sub eax, ecx
		mov esi, info_buffer
		mov esi, (huffman_buffer ptr [esi]).buffer
		add esi, eax
		pushad
		invoke crt_fputc, byte ptr [esi], file_stream
		popad

		loop L1
	.endif

	.if data_buffer != 0
		mov eax, data_buffer
		mov eax, (huffman_buffer ptr [eax]).current_byte_size
		mov ebx, eax
		sar eax, 3
		and ebx, 00000007h
		.if ebx != 0
			inc eax
		.endif

		mov count, eax
		.if count == 0
			jmp quit
		.endif
		mov ecx, count
L2:
		mov eax, count
		sub eax, ecx
		mov esi, data_buffer
		mov esi, (huffman_buffer ptr [esi]).buffer
		add esi, eax
		pushad
		invoke crt_fputc, byte ptr [esi], file_stream
		popad

		loop L2
	.endif
quit:
	pushad
	invoke crt_fclose, file_stream
	popad

	ret
write_into_file endp


read_from_file proc uses ecx esi, info_buffer: ptr ptr huffman_buffer, data_buffer: ptr ptr huffman_buffer, file_name: ptr byte
	local file_stream: dword
	local tmpc: byte
	local m_size: sdword

	mov file_stream, 0
	pushad
	invoke crt_fopen, file_name, OFFSET rb_mode
	mov file_stream, eax
	popad
	.if file_stream == 0
		mov eax, 0
		ret
	.endif

	.if info_buffer != 0
		invoke huffman_buffer_create, DECODE_INFO_BUFFER_SIZE
		mov esi, info_buffer
		mov dword ptr [esi], eax

		mov ecx, DECODE_INFO_BUFFER_SIZE
L1:
		mov eax, DECODE_INFO_BUFFER_SIZE
		sub eax, ecx

		mov esi, info_buffer
		mov esi, dword ptr [esi]
		mov esi, (huffman_buffer ptr [esi]).buffer
		add esi, eax

		pushad
		invoke crt_fgetc, file_stream
		mov tmpc, al
		popad
		mov al, tmpc
		mov byte ptr [esi], al
		loop L1
	.endif

	.if data_buffer != 0
		mov esi, info_buffer
		mov esi, dword ptr [esi]
		mov esi, (huffman_buffer ptr [esi]).buffer
		add esi, 4
		mov esi, sdword ptr [esi]
		mov m_size, esi

		invoke huffman_buffer_create, m_size
		mov esi, data_buffer
		mov dword ptr [esi], eax

		mov ecx, m_size
L2:
		mov eax, m_size
		sub eax, ecx

		mov esi, data_buffer
		mov esi, dword ptr [esi]
		mov esi, (huffman_buffer ptr [esi]).buffer
		add esi, eax

		pushad
		invoke crt_fgetc, file_stream
		mov tmpc, al
		popad
		mov al, tmpc
		mov	byte ptr [esi], al
		loop L2
	.endif

	invoke crt_fclose, file_stream
	mov eax, 1
	ret
read_from_file endp


rebuild_pq proc uses ebx ecx esi, info_buffer: ptr huffman_buffer
	local q: dword
	local nodes: dword
	local data_offset: dword
	local i: dword

	invoke crt_malloc, 256 * 4
	mov nodes, eax
	invoke pq_create
	mov q, eax
	mov esi, info_buffer
	mov eax, (huffman_buffer ptr [esi]).buffer
	add eax, 24
	mov data_offset, eax
	mov ecx, 256
L1:
	mov ebx, 256
	sub ebx, ecx
	mov i, ebx

	push ecx
	invoke huffman_node_create_external_node, ebx
	pop ecx

	imul ebx, 4
	add ebx, nodes
	
	mov (dword ptr [ebx]), eax
	mov esi, eax
	mov eax, i
	imul eax, 4
	add eax, data_offset
	mov eax, dword ptr [eax]
	mov (huffman_node ptr [esi]).weight, eax
	loop L1

	mov ecx, 256
L2:
	mov eax, 256
	sub eax, ecx
	push ecx
	imul eax, 4
	add eax, nodes
	mov eax, (dword ptr [eax])
	invoke pq_insert, q, eax
	pop ecx
	loop L2

	invoke crt_free, nodes

	mov eax, q
	ret
rebuild_pq endp


huffman_buffer_get_next_bit proc uses ebx ecx esi, data_buffer: ptr huffman_buffer
	mov esi, data_buffer
	mov eax, (huffman_buffer ptr [esi]).buffer
	mov ebx, (huffman_buffer ptr [esi]).current_byte_size
	mov ecx, ebx
	sar ebx, 3
	add eax, ebx
	mov bl, byte ptr [eax]

	and ecx, 00000007h
	mov eax, 1
	.if ecx != 0
L1:
		sal eax, 1
		loop L1
	.endif

	and al, bl
	.if al != 0
		mov al, 1
	.endif
	and eax, 000000FFh
	inc (huffman_buffer ptr [esi]).current_byte_size
	ret
huffman_buffer_get_next_bit endp


huffman_buffer_get_next_byte proc, data_buffer: ptr huffman_buffer, forest: ptr huffman_node
	local current: dword
	local bit: byte
	invoke huffman_buffer_get_next_bit, data_buffer
	.if eax != 0
		mov eax, forest
		mov eax, (huffman_node ptr [forest]).right_child
		mov current, eax
	.else
		mov eax, forest
		mov current, eax
	.endif
	
L1:
	mov eax, current
	mov eax, (huffman_node ptr [eax]).left_child
	.if eax == 0
		mov eax, current
		mov eax, (huffman_node ptr [eax]).right_child
		.if eax == 0
			jmp quit
		.endif
	.endif

	invoke huffman_buffer_get_next_bit, data_buffer
	mov bit, al
	.if bit == 0
		mov eax, current
		mov eax, (huffman_node ptr [eax]).left_child
		mov current, eax
	.else
		mov eax, current
		mov eax, (huffman_node ptr [eax]).right_child
		mov current, eax
	.endif
	jmp L1
quit:
	mov eax, current
	mov al, (huffman_node ptr [eax]).key
	and eax, 000000FFh
	ret	
huffman_buffer_get_next_byte endp


decompress_into_buffer proc uses ebx ecx esi, info_buffer: ptr huffman_buffer, data_buffer: ptr huffman_buffer, forest: ptr huffman_node
	local decompressed_size: sdword
	local decompressed_buffer: dword
	local char: byte

	mov eax, info_buffer
	mov eax, (huffman_buffer ptr [eax]).buffer
	mov eax, sdword ptr [eax]
	mov decompressed_size, eax

	invoke huffman_buffer_create, decompressed_size
	mov decompressed_buffer, eax

	mov ecx, decompressed_size
L1:
	mov ebx, decompressed_size
	sub ebx, ecx
	
	push ebx
	invoke huffman_buffer_get_next_byte, data_buffer, forest
	pop ebx
	mov char, al

	mov esi, decompressed_buffer
	mov esi, (huffman_buffer ptr [esi]).buffer
	; add esi, ebx
	mov byte ptr [esi + ebx], al

	inc ebx
	sal ebx, 3

	mov esi, decompressed_buffer
	mov (huffman_buffer ptr [esi]).current_byte_size, ebx
	loop L1

	mov eax, decompressed_buffer
	ret
decompress_into_buffer endp

end
