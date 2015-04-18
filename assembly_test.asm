global start
 
section .text
start:

  ; push the character onto the stack
  push 'A'
  call print
  
  ;exit
  push dword 0
  mov eax,1
  sub esp,12
  int 0x80
  add esp,4

; print a character then a newline
print:
  push dword [esp+4]
  call putchar
  push 0x0A
  call putchar
  add esp,8
  ret

; print a single character to stdout
putchar:  

  ; save the buffer location
  push dword [esp+4]
  mov edx,esp

  ; push the arguments onto the stack
  push dword 1   ; stdout
  push dword edx ; buffer location
  push dword 1   ; size
  mov eax,4      ; write
  sub esp,4

  ; call the syscall
  int 0x80
  add esp,16

  ; pop the buffer location
  add esp,4

  ret
