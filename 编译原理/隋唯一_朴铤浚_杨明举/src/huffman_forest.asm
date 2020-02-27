;---------------------------
;this file inplements the functions
;which are similar to those in dsa
;created by swy 2019/10/14
;---------------------------


include huffman_forest.inc

.data
rb_flag byte "rb", 0

.code
; return ptr priority_queue
char_statistics proc uses ebx ecx edx esi, file_name: ptr byte, byte_count: ptr dword
	local q: ptr priority_queue, nodes: ptr ptr huffman_node, file_stream: dword, char: dword
	invoke crt_malloc, 1024
	mov nodes, eax

	mov ebx, byte_count
	mov ecx, 0
	mov [ebx + 0], ecx
	
	invoke pq_create
	mov q, eax
	mov ecx, 256
	mov ebx, 0
create_external_node:
	invoke huffman_node_create_external_node, ebx
	mov edx, [nodes + 0]
	mov [edx + ebx * 4 + 0], eax
	inc ebx
	loop create_external_node

	mov file_stream, 0
	invoke crt_fopen, file_name, offset rb_flag
	mov file_stream, eax
	cmp eax, 0
	je release

	mov char, 0
read_byte:
	invoke crt_fgetc, file_stream
	mov char, eax
	.if eax == 0ffffffffh
		jmp build_pq
	.endif
	mov edx, [nodes + 0]
	mov esi, [edx + eax * 4]
	mov ecx, [esi + 1]
	inc ecx
	mov [esi + 1], ecx

	mov edx, byte_count
	mov ecx, [edx]
	inc ecx
	mov [edx], ecx
	jmp read_byte

build_pq:
	invoke crt_fclose, file_stream
	mov ecx, 256
	mov ebx, 0
build:
	mov edx, nodes
	invoke pq_insert, q, [edx + 4 * ebx]
	inc ebx
	loop build

	invoke crt_free, nodes	
	mov eax, q
	ret

release:
	mov eax, 0
	ret
char_statistics endp 


; return ptr huffman_node
huffman_forest_create proc uses ebx ecx, q: ptr priority_queue
	local first: ptr huffman_node, second: ptr huffman_node
	mov ebx, q
	.if ebx == 0
		jmp ret_0
	.endif
	mov ecx, [ebx]
	.if ecx == 0
		jmp ret_0
	.endif
build:
	mov ecx, [ebx]
	.if ecx <= 1
		jmp done
	.endif
	invoke pq_delmax, q
	mov first, eax
	invoke pq_delmax, q
	mov second, eax
	invoke huffman_node_create_internal_node, first, second
	invoke pq_insert, q, eax	
	jmp build
done:
	invoke pq_delmax, q
	ret
ret_0: 
	mov eax, 0
	ret
huffman_forest_create endp 


; return nothing
huffman_forest_destroy proc uses ebx edx, forest: ptr huffman_node
	.if forest == 0
		ret
	.endif
	mov ebx, forest
	mov edx, [ebx + 5]
	.if edx != 0
		invoke huffman_forest_destroy, edx
	.endif
	mov edx, [ebx + 9]
	.if edx != 0
		invoke huffman_forest_destroy, edx
	.endif
	invoke huffman_node_destroy, forest
	ret
huffman_forest_destroy endp 


end
