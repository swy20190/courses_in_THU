;-----------------------------
;this file is created to define
;some functionname for huffman_forest.
;created by swy 2019/10/14
;-----------------------------




include priority_queue.inc
include msvcrt.inc
includelib msvcrt.lib

; return ptr priority_queue
char_statistics proto, file_name: ptr byte, byte_count: ptr dword

; return ptr huffman_node
huffman_forest_create proto, q: ptr priority_queue

; return nothing
huffman_forest_destroy proto, forest: ptr huffman_node
