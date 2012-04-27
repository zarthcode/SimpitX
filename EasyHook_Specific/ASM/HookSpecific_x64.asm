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
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


.CODE



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; Trampoline_ASM_x64
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; This method is highly optimized and executes within 78 nanoseconds
; including the intro, outro and return...
; "IsExecuted" has to be within the next code subpage to prevent the
; Self-Modifing-Code-Condition to apply which would reduce performance 
; about 200 ns.

; Only for comparsion: The first proof of concept was unoptimized and
; did execute within 10000 nanoseconds... This optimized version just
; uses RIP relative addressing instead of register relative addressing,
; prevents the SMC condition and uses RIP relative jumps...


public Trampoline_ASM_x64

Trampoline_ASM_x64 PROC

NETIntro:
	;void*			NETEntry; // fixed 0 (0) 
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	
OldProc:
	;BYTE*			OldProc; // fixed 4 (8)  
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	
NewProc:
	;BYTE*			NewProc; // fixed 8 (16) 
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	
NETOutro:
	;void*			NETOutro; // fixed 12 (24) 
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	
IsExecutedPtr:
	;size_t*		IsExecutedPtr; // fixed 16 (32) 
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	db 0
	
; ATTENTION: 64-Bit requires stack alignment (RSP) of 16 bytes!!
	mov rax, rsp
	push rcx ; save not sanitized registers...
	push rdx
	push r8
	push r9
	mov r8, rax ; InitialRSP for NETIntro()...
	
	sub rsp, 4 * 16 ; space for SSE registers
	
	movups [rsp + 3 * 16], xmm0
	movups [rsp + 2 * 16], xmm1
	movups [rsp + 1 * 16], xmm2
	movups [rsp + 0 * 16], xmm3
	
	sub rsp, 32; shadow space for method calls
	
	lea rax, [IsExecutedPtr]
	mov rax, [rax]
	db 0F0h ; interlocked increment execution counter
	inc qword ptr [rax]
	
; is a user handler available?
	cmp qword ptr[NewProc], 0
	
	db 3Eh ; branch usually taken
	jne CALL_NET_ENTRY
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; call original method
		lea rax, [IsExecutedPtr]
		mov rax, [rax]
		db 0F0h ; interlocked decrement execution counter
		dec qword ptr [rax]
		
		lea rax, [OldProc]
		jmp TRAMPOLINE_EXIT

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; call hook handler or original method...
CALL_NET_ENTRY:	

	
; call NET intro
	lea rcx, [IsExecutedPtr + 8] ; Hook handle (only a position hint)
	mov rdx, qword ptr [rsp + 32 + 4 * 16 + 4 * 8] ; push return address
	call qword ptr [NETIntro] ; Hook->NETIntro(Hook, RetAddr, InitialRSP);
	
; should call original method?
	test rax, rax
	
	db 3Eh ; branch usually taken
	jne CALL_HOOK_HANDLER
	
	; call original method
		lea rax, [IsExecutedPtr]
		mov rax, [rax]
		db 0F0h ; interlocked decrement execution counter
		dec qword ptr [rax]
	
		lea rax, [OldProc]
		jmp TRAMPOLINE_EXIT
		
CALL_HOOK_HANDLER:
; adjust return address
	lea rax, [CALL_NET_OUTRO]
	mov qword ptr [rsp + 32 + 4 * 16 + 4 * 8], rax

; call hook handler
	lea rax, [NewProc]
	jmp TRAMPOLINE_EXIT 

CALL_NET_OUTRO: ; this is where the handler returns...

; call NET outro
	push 0 ; space for return address
	push rax
	
	sub rsp, 32 + 16; shadow space for method calls and SSE registers
	movups [rsp + 32], xmm0
	
	lea rcx, [IsExecutedPtr + 8]  ; Param 1: Hook handle hint
	lea rdx, [rsp + 56] ; Param 2: Address of return address
	call qword ptr [NETOutro] ; Hook->NETOutro(Hook);
	
	lea rax, [IsExecutedPtr]
	mov rax, [rax]
	db 0F0h ; interlocked decrement execution counter
	dec qword ptr [rax]
	
	add rsp, 32 + 16
	movups xmm0, [rsp - 16]
	
	pop rax ; restore return value of user handler...
	
; finally return to saved return address - the caller of this trampoline...
	ret
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; generic outro for both cases...
TRAMPOLINE_EXIT:

	add rsp, 32 + 16 * 4

	movups xmm3, [rsp - 4 * 16]
	movups xmm2, [rsp - 3 * 16]
	movups xmm1, [rsp - 2 * 16]
	movups xmm0, [rsp - 1 * 16]
	
	pop r9
	pop r8
	pop rdx
	pop rcx
	
	jmp qword ptr[rax] ; ATTENTION: In case of hook handler we will return to CALL_NET_OUTRO, otherwise to the caller...
	
	
; outro signature, to automatically determine code size
	db 78h
	db 56h
	db 34h
	db 12h

Trampoline_ASM_x64 ENDP


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; HookInjectionCode_ASM_x64
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
public Injection_ASM_x64

Injection_ASM_x64 PROC
; no registers to save, because this is the thread main function
	mov         r14, rcx ; save parameter to non-volatile register
	sub         rsp, 40  ; space for register parameter stack, should be 32 bytes... no idea why it only works with 40
	
; call LoadLibraryW(Inject->EasyHookPath);
	mov         rcx, qword ptr [r14 + 8]
	call        qword ptr [r14 + 40] ; LoadLibraryW
	mov			r13, rax
	test		rax, rax
	je			HookInject_FAILURE_A
	
; call GetProcAddress(hModule, Inject->EntryPoint)
	mov         rdx, qword ptr [r14 + 24] 
	mov         rcx, rax 
	call        qword ptr [r14 + 56] ; GetProcAddress 
	test		rax, rax
	je			HookInject_FAILURE_B
	
; call EasyHookEntry(Inject);
	mov			rcx, r14
	call		rax
	mov			r15, rax ; save error code to non-volatile register

; call FreeLibrary(hEasyHookLib)
	mov			rcx, r13
	call		qword ptr [r14 + 48] ; FreeLibrary
	test		rax, rax
	je			HookInject_FAILURE_C
	
	jmp			HookInject_EXIT
	
HookInject_FAILURE_A:
	call		qword ptr [r14 + 88] ; GetLastError
	or			rax, 40000000h
	jmp			HookInject_FAILURE_E
HookInject_FAILURE_B:
	call		qword ptr [r14 + 88] ; GetLastError
	or			rax, 10000000h
	jmp			HookInject_FAILURE_E	
HookInject_FAILURE_C:
	call		qword ptr [r14 + 88] ; GetLastError
	or			rax, 30000000h
	jmp			HookInject_FAILURE_E	
HookInject_FAILURE_E:
	mov			r15, rax ; save error value
	
HookInject_EXIT:

; call VirtualProtect(Outro, 8, PAGE_EXECUTE_READWRITE, &OldProtect)
	lea			rbx, qword ptr [rsp + 8] ; writes into register parameter stack
	mov			r9, rbx
	mov			r8, 40h
	mov			rdx, 8
	mov			rcx, rbx
	call		qword ptr [r14 + 72] ; VirtualProtect
	test		rax, rax
	
	jne HookInject_EXECUTABLE

	; failed to make stack executable
		call		qword ptr [r14 + 88] ; GetLastError
		or			rax, 01000000h
		mov			rcx, rax
		call		qword ptr [r14 + 80] ; ExitThread
		
HookInject_EXECUTABLE:
; save outro to executable stack
	mov			rbx, [r14 + 64] ; VirtualFree()
	mov			rbp, [r14 + 80] ; ExitThread()
	
	mov			rax, 000D5FFCF8B49D3FFh
		; call rbx
		; mov rcx, r15
		; call rbp
		
	mov qword ptr [rsp + 8], rax
	
; save params for VirtualFree(Inject->RemoteEntryPoint, 0, MEM_RELEASE);
	mov r8, 8000h
	mov rdx, 0h
	mov rcx, qword ptr [r14 + 32]
	
	lea rax, qword ptr [rsp + 8]
	sub rsp, 40
	jmp rax
	
; outro signature, to automatically determine code size
	db 78h
	db 56h
	db 34h
	db 12h

Injection_ASM_x64 ENDP

END