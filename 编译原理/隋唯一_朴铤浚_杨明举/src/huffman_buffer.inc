;------------------------------
;this head file named h_buffer is created to get ready 
;for the file to be compressed/decompressed
;created by swy 2019/10/14
;------------------------------



.model flat, c

include mapper.inc
include huffman_forest.inc
include msvcrt.inc
includelib msvcrt.lib

huffman_buffer struct
	current_byte_size 	sdword 0
	byte_capacity 		sdword 0
	buffer 				dword 0
huffman_buffer ends

DECODE_INFO_BUFFER_SIZE = 2 * 4 + 16 + 4 * 256

huffman_buffer_create proto, max_byte_count: sdword

huffman_buffer_destroy proto, buffer: ptr huffman_buffer

huffman_buffer_insert proto, buffer: ptr huffman_buffer, bit_count: sdword, bits: ptr byte

get_compressed_size proto, buffer: ptr huffman_buffer

rebuild_pq proto, info_buffer: ptr huffman_buffer

compress_into_buffer proto, file_name: ptr byte, buffer: ptr huffman_buffer, mappers: ptr mapper

save_encode_info_into_buffer proto, buffer: ptr huffman_buffer, decompressed_size: sdword, compressed_size: sdword, password: ptr byte, mappers: ptr mapper

write_into_file proto, info_buffer: ptr huffman_buffer, data_buffer: ptr huffman_buffer, file_name: ptr byte

read_from_file proto, info_buffer: ptr ptr huffman_buffer, data_buffer: ptr ptr huffman_buffer, file_name: ptr byte

huffman_buffer_get_next_bit proto, data_buffer: ptr huffman_buffer

huffman_buffer_get_next_byte proto, data_buffer: ptr huffman_buffer, forest: ptr huffman_node

decompress_into_buffer proto, data_buffer: ptr huffman_buffer, info_buffer: ptr huffman_buffer, forest: ptr huffman_node
