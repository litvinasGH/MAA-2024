.586P
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib MAA2024-L.lib
EXTRN ruslang: proc
EXTRN writeInt: proc
EXTRN showInt: proc
EXTRN today: proc
EXTRN showtime: proc
ExitProcess PROTO :DWORD


.stack 4096

.const
	newtrue BYTE 1
	new0x2 BYTE 2
	new0b11 BYTE 3
	new2 BYTE 2
	new1 BYTE 1
	new3 BYTE 3

	new0 BYTE 0

.data
	buffer BYTE 256 dup(0)
	buffecx DWORD ?
	kaz DWORD 3
	xio DWORD 1
.code

fi PROC x :  DWORD, y :  DWORD
	pop ebx 
	pop eax 
	mov cl, bl
	shl eax, cl
	push eax
	push func
	jmp local0
local0:
	pop eax
	ret
fi ENDP

func PROC x :  DWORD, ko :  DWORD
	pop ebx 
	pop eax 
	mov cl, bl
	shr eax, cl
	push eax
	push 1
	jmp local1
local1:
	pop eax
	ret
func ENDP

main PROC
	push kaz
	pop ebx 
	pop eax 
	mov cl, bl
	shr eax, cl
	push eax
	push xio
	push 3
	push 3
	mov ecx, writeInt
cycle0:
	mov buffecx, ecx
	mov ecx, fi
	mov buffecx, ecx
	pop ebx 
	pop eax 
	mov cl, bl
	shl eax, cl
	push eax
	mov ecx, buffecx
loop cycle0
	push 0
		jmp theend
theend:
	call today
	call showtime
call ExitProcess
main ENDP
end main