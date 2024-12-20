.586P
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib MAA2024-L.lib
ExitProcess PROTO :DWORD

EXTRN showtime: proc
EXTRN ruslang: proc
EXTRN today: proc
EXTRN showInt: proc

.stack 4096

.const
	L0 BYTE 2
	L1 BYTE 3
	L2 BYTE 1
	L3 BYTE 2
	L4 BYTE 3
	L5 BYTE 2
	L6 BYTE 1
	L7 BYTE 1
	L8 BYTE 3
	L9 BYTE 3
	L10 BYTE 4
	L11 BYTE 2
	L12 BYTE 3
	L13 BYTE 1
	L14 BYTE 0

.data
	buffer BYTE 256 dup(0)
	buffecx DWORD ?
	kazm BYTE 0
	flagm BYTE 0
	xm BYTE 0

.code

fi PROC x : BYTE, y : BYTE
	mov al, L0
	push ax
	mov al, L0
	push ax
	pop ebx 
	pop eax 
	mov cl, bl
	shl eax, cl
	push eax
	pop ax
	mov L0, al
	mov al, func
	push ax
	jmp local0
local0:
	pop eax
	ret
fi ENDP

func PROC x : BYTE, ko : BYTE
	mov al, L1
	push ax
	mov al, L1
	push ax
	pop ebx 
	pop eax 
	mov cl, bl
	shr eax, cl
	push eax
	pop ax
	mov L1, al
	mov al, L2
	push ax
	jmp local1
local1:
	pop eax
	ret
func ENDP

main PROC
	call ruslang
	pop ax
	mov kazm, al
	pop ax
	mov L4, al
	mov al, L5
	push ax
	mov al, L6
	push ax
	pop ebx 
	pop eax 
	mov cl, bl
	shr eax, cl
	push eax
	pop ax
	mov L5, al
	pop ax
	mov L7, al
	pop ax
	mov xm, al
	mov al, L9
	push ax
	pop ax
	mov L9, al
	call today
	push eax
	mov al, L11
	push ax
	call showInt
	push eax
mov cl, L11
movzx ecx, cl
cycle0:
	mov buffecx, ecx
	mov al, L12
	push ax
	mov al, L13
	push ax
	pop ebx 
	pop eax 
	mov cl, bl
	shl eax, cl
	push eax
	pop ax
	mov L12, al
	mov ecx, buffecx
loop cycle0
	mov al, L14
	push ax
		jmp theend
theend:
call ExitProcess
main ENDP
end main