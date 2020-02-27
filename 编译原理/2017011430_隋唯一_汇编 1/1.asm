.386
.model flat, stdcall
option casemap :none
include windows.inc
include kernel32.inc
include masm32.inc
includelib kernel32.lib
includelib masm32.lib
.data
info db "姓名：隋唯一 班级：软件71 性别：男 ID：2017011430", 0
.code
start:
invoke StdOut, addr info
invoke ExitProcess, 0
end start
