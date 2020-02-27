;-------------------------------------
;this file is created to declare
;the huffman_tree node struct
;created by swy 2019/10/12
;-------------------------------------





.model flat, c

include msvcrt.inc
includelib msvcrt.lib

huffman_node struct
	key				byte  ?
	weight			dword ?
	left_child		dword ?
	right_child		dword ?
huffman_node ends

; return ptr huffman_node
huffman_node_create_external_node proto, n: dword

; return ptr huffman_node
huffman_node_create_internal_node proto, left_child: ptr huffman_node, right_child: ptr huffman_node

; return nothing
huffman_node_destroy proto, node: ptr huffman_node
