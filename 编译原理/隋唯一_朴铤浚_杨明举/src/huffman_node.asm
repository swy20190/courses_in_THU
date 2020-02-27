;---------------------------------
;this file is created to inplement
;the method of tree node(visit,create,destroy)
;created by swy 2019/10/12
;last modified by swy 2019/10/19
;--------------------------------


include huffman_node.inc

.code

; return ptr huffman_node
huffman_node_create_external_node proc uses ecx, n: dword
	invoke crt_malloc, size huffman_node
	mov ecx, n 
	mov (huffman_node ptr [eax]).key, cl
	mov (huffman_node ptr [eax]).weight, 0
	mov (huffman_node ptr [eax]).left_child, 0
	mov (huffman_node ptr [eax]).right_child, 0
	ret
huffman_node_create_external_node endp


; return ptr huffman_node
huffman_node_create_internal_node proc uses ebx ecx, left_child: ptr huffman_node, right_child: ptr huffman_node
	invoke crt_malloc, size huffman_node
	mov (huffman_node ptr [eax]).key, 0
	mov ecx, 0
	mov ebx, left_child
	mov ecx, (huffman_node ptr [ebx]).weight
	mov ebx, right_child
	add ecx, (huffman_node ptr [ebx]).weight

	; mov ecx, (huffman_node ptr [left_child]).weight
	; add ecx, (huffman_node ptr [right_child]).weight
	mov (huffman_node ptr [eax]).weight, ecx
	mov ebx, left_child
	mov (huffman_node ptr [eax]).left_child, ebx
	mov ebx, right_child
	mov (huffman_node ptr [eax]).right_child, ebx
	ret
huffman_node_create_internal_node endp


; return nothing
huffman_node_destroy proc, node: ptr huffman_node
	.if node != 0
		invoke crt_free, node
	.endif
	ret
huffman_node_destroy endp

end
