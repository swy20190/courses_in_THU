;--------------------------
;main.asm created by swy 2019/10/10
;--------------------------


.model flat, c

include msvcrt.inc
include huffman_node.inc
include priority_queue.inc
include huffman_forest.inc
include mapper.inc
include huffman_buffer.inc
include md5.inc

usage proto

compress proto, file_name: ptr byte, password: ptr byte

decompress proto, file_name: ptr byte, password: ptr byte

.data
nl EQU <0dh, 0ah>
usage_msg byte				"WELCOME TO OUR FILE COMPRESSOR!",nl,
                            "[      Usage     ]:", nl,
							"  - compress   : compress the file into a .swy file.",nl,
							"  - decompress : decompress a .swy file.", nl,
							"  - quit       : quit the program.", nl, 0
failed_no_such_file byte	"[     FAILED     ]: No such file named [ %s ] ...", nl, 0
info_compressing byte		"[      INFO      ]: Compressing ...", nl, 0
info_compressed_into byte	"[      INFO      ]: File [ %s ] was compressed into [ %s ] successfully...", nl, 0
failed_no_permission byte	"[     FAILED     ]: You have NO permission to decompress the file.", nl, 0
info_decompressing byte		"[      INFO      ]: Decompressing ...", nl, 0
info_decompressed_into byte "[      INFO      ]: File [ %s ] was decompressed into [ %s ] successfully...", nl, 0

tql byte					".swy", 0

before byte					1024 dup(0)
after byte					1024 dup(0)
passwd byte					256 dup(0)
cmd byte					256 dup(0)
compress_str byte				"compress", 0
decompress_str byte				"decompress", 0
quit_str byte					"quit", 0


input_msg byte				"[ Input Command  ]: ", 0
mask_msg byte				"%s", 0
no_cmd_msg byte				"[      INFO      ]: Only compress, decompress and quit commands are supported.", nl, 0
input_filename_msg byte		"[ Input Filename ]: ", 0
input_passwd_msg byte		"[ Input Password ]: ", 0
quit_msg byte				"[      INFO      ]: The program is going to exit, press 'Enter' to continue."


.code

compress proc uses eax ebx edx, file_name: ptr byte, password: ptr byte
	local md5_code: ptr byte, decompressed_size: sdword,
		q: ptr pq, forest: ptr huffman_forest, mappers: ptr mapper,
		data_buffer: ptr huffman_buffer, compressed_size: sdword,
		info_buffer: ptr huffman_buffer, compressed_file_name: ptr byte

	invoke md5, password
	mov md5_code, eax
	mov decompressed_size, 0
	invoke char_statistics, file_name, addr decompressed_size
	mov q, eax
	.if q == 0
		invoke crt_printf, addr failed_no_such_file, file_name
		ret
	.endif
	invoke crt_printf, addr info_compressing
	invoke huffman_forest_create, q
	mov forest, eax
	invoke mapper_init
	mov mappers, eax
	invoke mapper_set_all, mappers, forest

	mov ebx, decompressed_size
	add ebx, ebx
	invoke huffman_buffer_create, ebx
	mov data_buffer, eax
	invoke compress_into_buffer, file_name, data_buffer, mappers
	invoke get_compressed_size, data_buffer
	mov compressed_size, eax
	invoke huffman_buffer_create, DECODE_INFO_BUFFER_SIZE
	mov info_buffer, eax
	invoke save_encode_info_into_buffer, info_buffer, decompressed_size, 
		compressed_size, md5_code, mappers

	invoke crt_strlen, file_name
	mov ebx, eax
	add ebx, 5
	invoke crt_malloc, ebx
	mov compressed_file_name, eax
	invoke crt_strcpy, compressed_file_name, file_name
	invoke crt_strcat, compressed_file_name, offset tql
	invoke write_into_file, info_buffer, data_buffer, compressed_file_name
	invoke crt_printf, offset info_compressed_into, file_name, compressed_file_name


	invoke crt_free, md5_code
	invoke pq_destroy, q
	invoke huffman_forest_destroy, forest
	invoke mapper_destroy, mappers
	invoke huffman_buffer_destroy, data_buffer
	invoke huffman_buffer_destroy, info_buffer
	invoke crt_free, compressed_file_name
	ret
compress endp


decompress proc, file_name: ptr byte, password: ptr byte
	local data_buffer: ptr huffman_buffer, info_buffer: ptr huffman_buffer,
		q: ptr pq, forest: ptr huffman_forest, decompressed_buffer: ptr huffman_buffer,
		mappers: ptr mapper, decompressed_file_name: ptr byte

	mov data_buffer, 0
	mov info_buffer, 0
	invoke read_from_file, addr info_buffer, addr data_buffer, file_name
	.if eax == 0
		invoke crt_printf, addr failed_no_such_file, file_name
		ret
	.endif
	invoke password_is_valid, password, info_buffer
	
	.if eax == 0
		invoke crt_printf, addr failed_no_permission
		invoke huffman_buffer_destroy, data_buffer
		invoke huffman_buffer_destroy, info_buffer
		ret
	.else
		invoke crt_printf, addr info_decompressing
	.endif

	invoke rebuild_pq, info_buffer
	mov q, eax
	invoke huffman_forest_create, q
	mov forest, eax
	invoke decompress_into_buffer, info_buffer, data_buffer, forest
	mov decompressed_buffer, eax
	invoke mapper_init
	mov mappers, eax
	invoke mapper_set_all, mappers, forest

	invoke crt_strlen, addr file_name
	mov ebx, eax
	inc ebx
	invoke crt_malloc, ebx
	mov decompressed_file_name, eax
	invoke crt_strcpy, decompressed_file_name, file_name
	invoke crt_strlen, file_name
	mov ebx, decompressed_file_name
	sub eax, 4
	mov byte ptr [ebx + eax], 0
	
	invoke write_into_file, 0, decompressed_buffer, decompressed_file_name
	invoke crt_printf, addr info_decompressed_into, file_name, decompressed_file_name
	ret
decompress endp


usage proc
	invoke crt_printf, addr usage_msg
	ret
usage endp


main proc
	;invoke compress, addr before, addr pswd
	;invoke decompress, addr after, addr pswd	
	invoke usage
begin:
	invoke crt_printf, addr input_msg
	invoke crt_scanf, addr mask_msg, addr cmd
	
	invoke crt_strcmp, addr cmd, addr compress_str
	cmp eax, 0
	je compress_tag

	invoke crt_strcmp, addr cmd, addr decompress_str
	cmp eax, 0
	je decompress_tag

	invoke crt_strcmp, addr cmd, addr quit_str
	cmp eax, 0
	je quit_tag

	invoke crt_printf, addr no_cmd_msg
	jmp begin

compress_tag:
	invoke crt_printf, addr input_filename_msg
	invoke crt_scanf, addr mask_msg, addr before

	invoke crt_printf, addr input_passwd_msg
	invoke crt_scanf, addr mask_msg, addr passwd

	invoke compress, addr before, addr passwd
	jmp begin
decompress_tag:
	invoke crt_printf, addr input_filename_msg
	invoke crt_scanf, addr mask_msg, addr after

	invoke crt_printf, addr input_passwd_msg
	invoke crt_scanf, addr mask_msg, addr passwd

	invoke decompress, addr after, addr passwd
	jmp begin
quit_tag:
	invoke crt_printf, addr quit_msg
	invoke crt_getchar	; get the 'return' key
	invoke crt_getchar	; wait for msg
	ret
main endp

end
