;-----------------------------
;this file inplements the priorityqueue
;(the heap)
;created by swy 2019/10/15
;-----------------------------


include priority_queue.inc

; macro's return value will be stored in eax
; in macros, only eax will be modified

parent macro i
	mov eax, i
	sub eax, 1
	shr eax, 1
endm parent


last_internal macro n
	push ebx
	mov  ebx, n
	sub	 ebx, 1
	parent ebx
	pop ebx
endm last_internal


lchild macro i
	mov eax, i
	shl eax, 1
	add eax, 1
endm lchild


rchild macro i
	mov eax, i
	add eax, 1
	shl eax, 1
endm rchild

; ---------------------------------------------------------------

.code

in_heap proc n: dword, i: dword
	mov eax, i
	.if eax < 0
	.else
		.if eax < n
			mov eax, 1
			ret
		.endif
	.endif
	mov eax, 0
	ret
in_heap endp


lchild_valid proc, n: dword, i: dword
	push ebx
	push ecx
	mov ebx, n
	mov ecx, i
	lchild ecx
	invoke in_heap, ebx, eax
	pop ecx
	pop ebx
	ret
lchild_valid endp


rchild_valid proc, n: dword, i: dword
	push ebx
	push ecx
	mov ebx, n
	mov ecx, i
	rchild ecx
	invoke in_heap, ebx, eax
	pop ecx
	pop ebx
	ret
rchild_valid endp


bigger proc, q: ptr priority_queue, i: dword, j: dword
	push ebx
	push ecx
	push edx
	
	mov eax, j  
	mov ecx, q  
	mov edx, [ecx + eax * 4 + 4]  
	mov eax, i  
	mov ecx, q  
	mov eax, [ecx + eax * 4 + 4]  
	mov ecx, [edx + 1] ; j
	mov ebx, [eax + 1] ; i 

	.if ecx < ebx
		mov eax, j
	.else
		mov eax, i
	.endif
	
	pop edx
	pop ecx
	pop ebx
	ret
bigger endp


properparent proc, q: ptr priority_queue, n: dword, i: dword
	push ebx
	invoke rchild_valid, n, i
	.if eax == 1
		lchild i
		invoke bigger, q, i, eax
		mov ebx, eax
		rchild i
		invoke bigger, q, ebx, eax
	.else
		invoke lchild_valid, n, i
		.if eax == 1
			lchild i
			invoke bigger, q, i, eax
		.else
			mov eax, i
		.endif
	.endif
	pop ebx
	ret
properparent endp

; ---------------------------------------------------------------


; return ptr priority_queue
pq_create proc uses ebx ecx
	mov ecx, size priority_queue
	invoke crt_malloc, (size priority_queue)
	mov (priority_queue ptr [eax]).node_size, 0
	mov ebx, eax
	add ebx, 4
	mov ecx, 256
set0:
	mov dword ptr [ebx], 0
	add ebx, 4
	loop set0
	ret
pq_create endp 


; return nothing
pq_destroy proc uses ebx ecx, q: ptr priority_queue
	mov ecx, (priority_queue ptr q).node_size
	mov ebx, q
	add ebx, 4
	cmp ecx, 0
	je done
node_free:
	add ebx, 4
	loop node_free
done:
	ret
pq_destroy endp


; return dword
pq_percolate_up proc uses ecx edx esi, q: ptr priority_queue, i: dword
	local j: dword, temp: ptr huffman_node
up:
	cmp	i, 0  
	jle done

	parent i
	mov dword ptr j, eax  
    ; if (q->nodes[j]->weight < q->nodesi->weight)
	mov eax, j  
	mov ecx, q  
	mov edx, [ecx + eax * 4 + 4]  
	mov eax, i  
	mov ecx, q  
	mov eax, [ecx + eax * 4 + 4]  
	mov ecx, [edx + 1]  
	cmp ecx, [eax + 1]  
	jge continue
	jmp done
continue:
    ; swap
    ; huffman_node * t = q->nodesi;
	mov eax, i  
	mov ecx, q  
	mov edx, [ecx + eax * 4 + 4]  
	mov temp, edx  
    ; q->nodesi = q->nodes[j];
	mov eax, i  
	mov ecx, q  
	mov edx, j  
	mov esi, q  
	mov edx, [esi + edx * 4 + 4]  
	mov [ecx + eax * 4 + 4], edx  
    ; q->nodesj= t;
	mov eax, j  
	mov ecx, q  
	mov edx, temp  
	mov [ecx + eax * 4 + 4], edx  
	mov eax, j  
	mov i, eax  
	jmp up  
done:
	mov eax,i 
	ret
pq_percolate_up endp 
	

; return dword
pq_percolate_down proc uses ecx edx esi, q: ptr priority_queue, n: dword, i: dword
	local j: dword, temp:dword
down:
	invoke properparent, q, n, i
	mov j, eax
	cmp i, eax
	je done
	; swap
    ; huffman_node * t = q->nodesi;
	mov eax, i  
	mov ecx, q  
	mov edx, [ecx + eax * 4 + 4]  
	mov temp, edx  
    ; q->nodesi = q->nodes[j];
	mov eax, i  
	mov ecx, q  
	mov edx, j 
	mov esi, q  
	mov edx, [esi + edx * 4 + 4]  
	mov  [ecx + eax * 4 + 4], edx  
    ; q->nodes[j]= t;
	mov eax, j 
	; q->nodes[j]= t;
	mov ecx, q  
	mov edx, temp  
	mov  [ecx + eax * 4 + 4], edx  
	; i = j;
	mov eax, j 
	mov i, eax  
	jmp down  
	done:
	mov eax, i  
	ret
pq_percolate_down endp 


; return nothing
pq_heapify proc, q: ptr priority_queue, n: dword
	local i: dword
	last_internal n
	mov i, eax
heapify:
	invoke in_heap, n, i
	cmp eax, 0
	je done
	invoke pq_percolate_down, q, n, i
	sub i, 1
	jmp heapify
done:
	ret
pq_heapify endp 


; return nothing
pq_insert proc uses ebx ecx edx, q: ptr priority_queue, node: ptr huffman_node
	mov ebx, q
	mov ecx, [ebx]
	mov edx, node
	mov [ebx + 4 + 4 * ecx], edx
	inc ecx
	mov [ebx], ecx
	dec ecx

	invoke pq_percolate_up, q, ecx
	ret
pq_insert endp 


; return ptr huffman_node
pq_delmax proc uses ebx ecx edx, q: ptr priority_queue
	local max: ptr huffman_node
	mov ebx, q
	
	; huffman_node * max = q->nodes[0];
	mov edx, [ebx + 4]
	mov max, edx

    ; q->nodes[0] = q->nodes[--(q->size)];
	mov ecx, [ebx + 0]
	dec ecx
	mov [ebx + 0], ecx
	mov edx, [ebx + 4 + 4 * ecx]
	mov [ebx + 4], edx

    ; pq_percolate_down(q, q->size, 0); 
	invoke pq_percolate_down, ebx, ecx, 0

    ; return max; 
	mov eax, max
	ret
pq_delmax endp 

end
