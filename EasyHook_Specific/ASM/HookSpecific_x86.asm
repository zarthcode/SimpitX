;Copyright (c) 2008 by Christoph Husse, SecurityRevolutions e.K.
;
;Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
;associated documentation files (the "Software"), to deal in the Software without restriction, 
;including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
;and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
;subject to the following conditions:
;
;The above copyright notice and this permission notice shall be included in all copies or substantial 
;portions of the Software.
;
;THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
;LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
;IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
;WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
;SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
;
;Visit http://www.codeplex.com/easyhook for more information.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.386
.model flat, c
.code


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; Trampoline_ASM_x86
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
public Trampoline_ASM_x86@0

Trampoline_ASM_x86@0 PROC

; Handle:		1A2B3C05h
; NETEntry:		1A2B3C03h
; OldProc:		1A2B3C01h
; NewProc:		1A2B3C00h
; NETOutro:		1A2B3C06h
; IsExecuted:	1A2B3C02h
; RetAddr:		1A2B3C04h
; Ptr:NewProc:	1A2B3C07h

	mov eax, esp
	push ecx ; both are fastcall parameters, ECX is also used as "this"-pointer
	push edx
	mov ecx, eax; InitialRSP value for NETIntro()...
	
	mov eax, 1A2B3C02h
	db 0F0h ; interlocked increment execution counter
	inc dword ptr [eax]
	
; is a user handler available?
	mov eax, 1A2B3C07h
	cmp dword ptr[eax], 0
	
	db 3Eh ; branch usually taken
	jne CALL_NET_ENTRY
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; call original method
		mov eax, 1A2B3C02h
		db 0F0h ; interlocked decrement execution counter
		dec dword ptr [eax]
		mov eax, 1A2B3C01h
		jmp TRAMPOLINE_EXIT

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; call hook handler or original method...
CALL_NET_ENTRY:	
	
; call NET intro
	push ecx
	push dword ptr [esp + 12] ; push return address
	push 1A2B3C05h ; Hook handle
	mov eax, 1A2B3C03h
	call eax ; Hook->NETIntro(Hook, RetAddr);
	
; should call original method?
	test eax, eax
	
	db 3Eh ; branch usually taken
	jne CALL_HOOK_HANDLER
	
	; call original method
		mov eax, 1A2B3C02h
		db 0F0h ; interlocked decrement execution counter
		dec dword ptr [eax]
		mov eax, 1A2B3C01h
		jmp TRAMPOLINE_EXIT
		
CALL_HOOK_HANDLER:
; adjust return address --- ATTENTION: this offset "83h" will also change if CALL_NET_OUTRO moves due to changes...
	mov dword ptr [esp + 8], 1A2B3C04h

; call hook handler
	mov eax, 1A2B3C00h
	jmp TRAMPOLINE_EXIT 

CALL_NET_OUTRO: ; this is where the handler returns...

; call NET outro --- ATTENTION: Never change EAX/EDX from now on!
	push 0 ; space for return address
	push eax
	push edx
	
	lea eax, [esp + 8]
	push eax ; Param 2: Address of return address
	push 1A2B3C05h ; Param 1: Hook handle
	mov eax, 1A2B3C06h
	call eax ; Hook->NETOutro(Hook);
	
	mov eax, 1A2B3C02h
	db 0F0h ; interlocked decrement execution counter
	dec dword ptr [eax]
	
	pop edx ; restore return value of user handler...
	pop eax
	
; finally return to saved return address - the caller of this trampoline...
	ret
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; generic outro for both cases...
TRAMPOLINE_EXIT:

	pop edx
	pop ecx
	
	jmp eax ; ATTENTION: In case of hook handler we will return to CALL_NET_OUTRO, otherwise to the caller...
	
; outro signature, to automatically determine code size
	db 78h
	db 56h
	db 34h
	db 12h

Trampoline_ASM_x86@0 ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; HookInjectionCode_ASM_x86
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
public Injection_ASM_x86
Injection_ASM_x86 PROC
; no registers to save, because this is the thread main function
; save first param (address of hook injection information)
	mov esi, dword ptr [esp + 4]
	
; call LoadLibraryW(Inject->EasyHookPath);
	push dword ptr [esi + 8]
	
	call dword ptr [esi + 40] ; LoadLibraryW@4
	mov ebp, eax
	test eax, eax
	je HookInject_FAILURE_A
	
; call GetProcAddress(eax, Inject->EasyHookEntry);
	push dword ptr [esi + 24]
	push ebp
	call dword ptr [esi + 56] ; GetProcAddress@8
	test eax, eax
	je HookInject_FAILURE_B
	
; call EasyHookEntry(Inject);
	push esi
	call eax
	push eax ; save error code

; call FreeLibrary(ebp)
	push ebp
	call dword ptr [esi + 48] ; FreeLibrary@4
	test eax, eax
	je HookInject_FAILURE_C
	jmp HookInject_EXIT
	
HookInject_FAILURE_A:
	call dword ptr [esi + 88] ; GetLastError
	or eax, 40000000h
	jmp HookInject_FAILURE_E
HookInject_FAILURE_B:
	call dword ptr [esi + 88] ; GetLastError
	or eax, 10000000h
	jmp HookInject_FAILURE_E	
HookInject_FAILURE_C:
	call dword ptr [esi + 88] ; GetLastError
	or eax, 30000000h
	jmp HookInject_FAILURE_E	
HookInject_FAILURE_E:
	push eax ; save error value
	
HookInject_EXIT:

	push 0
	push 0
	push 0; // shadow space for executable stack part...

; call VirtualProtect(Outro, 4, PAGE_EXECUTE_READWRITE, &OldProtect)
	lea ebx, dword ptr [esp + 8] ; we'll write to shadow space
	push ebx
	push 40h
	push 12
	push ebx
	call dword ptr [esi + 72] ; VirtualProtect@16
	test eax, eax
	
	jne HookInject_EXECUTABLE

	; failed to make stack executable
		call dword ptr [esi + 88] ; GetLastError
		or eax, 20000000h
		add esp, 16
		ret
		
HookInject_EXECUTABLE:
; save outro to executable stack
	mov dword ptr [esp],	 0448BD3FFh		; call ebx [VirtualFree()]
	mov dword ptr [esp + 4], 05C8B0C24h		; mov eax, [esp + 12]
	mov dword ptr [esp + 8], 0E3FF1024h		; mov ebx, [esp + 16]
											; jmp ebx [exit thread]
	
; save params for VirtualFree(Inject->RemoteEntryPoint, 0, MEM_RELEASE);
	mov ebx, [esi + 64] ; VirtualFree()
	push 08000h
	push 0
	push dword ptr [esi + 16]
	
	lea eax, dword ptr [esp + 12]
	jmp eax
	
; outro signature, to automatically determine code size
	db 78h
	db 56h
	db 34h
	db 12h

Injection_ASM_x86 ENDP

END