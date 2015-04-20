global start
section .text

putint:
push eax
push ebx
push ecx
push edx
mov eax,dword[esp+20]
mov ecx,0
push 0
push 0x0A
mov ebx,10
.putint0:
add ecx,1
mov dx,0
div bx
add edx,0x30
push edx
cmp ax,0
jz .putint1
jmp .putint0
.putint1:
push dword esp
call putstr
add esp,4
lea ecx,[8+4*ecx]
add esp,ecx
pop edx
pop ecx
pop ebx
pop eax
ret

putstr:
push eax
mov esi,dword[esp+8]
.putstr0:
mov byte al,[esi]
cmp al,0
je .putstr1
push eax
call putchar
add esp,4
add esi,4
jmp .putstr0
.putstr1:
pop eax
ret

putchar:
push dword [esp+4]
mov edx,esp
push dword 1
push dword edx
push dword 1
mov eax,4
sub esp,4
int 0x80
add esp,20
ret

start:
mov ebp,esp
sub esp,8
mov eax,5
mov ebx,3
add eax,ebx
mov [ebp-4],eax
mov eax,1
sub [ebp-4],eax
push dword [ebp-4]
call putint
mov eax,10
push eax
push edx
mov eax,eax
mul dword [ebp-4]
push eax
add esp,4
pop edx
pop eax
mov eax,[esp-12]
mov [ebp-8],eax
push dword [ebp-8]
call putint
add esp,8

;exit
push dword 0
mov eax,1
sub esp,12
int 0x80
add esp,4
