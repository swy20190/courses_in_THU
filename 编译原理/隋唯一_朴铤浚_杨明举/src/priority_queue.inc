;---------------------------------
;ths file declares the priority_queue
;data-structure which is to be used in 
;the huffman_coding algorithm
;created by swy 2019/10/15
;---------------------------------


.model flat, c

.model flat, c

include huffman_node.inc
include msvcrt.inc
includelib msvcrt.lib

priority_queue struct
	node_size		dword ?
	nodes			dword 256 dup(?)
priority_queue ends

; return ptr priority_queue
pq_create proto

; return nothing
pq_destroy proto, q: ptr priority_queue
	
; return dword
pq_percolate_up proto, q: ptr priority_queue, i: dword

; return dword
pq_percolate_down proto, q: ptr priority_queue, n: dword, i: dword

; return nothing
pq_heapify proto, q: ptr priority_queue, n: dword

; return nothing
pq_insert proto, q: ptr priority_queue, node: ptr huffman_node

; return ptr huffman_node
pq_delmax proto, q: ptr priority_queue
