; *****************************************************************
; Description: File IO using command line args to find the number of times a word is found
; *****************************************************************

; clobbers rdx
%macro findLength 1
    mov rdx, 0  ; set counter to 0
    %%obtainLength:
        ; increment counter until null is found
        cmp byte[%1+rdx], NULL
        je %%endLength
        inc rdx
        jmp %%obtainLength
    %%endLength:
    ; once null is found, finish the macro

%endmacro

; clobbers rax, rdi, rsi
%macro cout 1

    ; Prints out the provided string
    mov rax, SYS_write
    mov rdi, STDOUT
    mov rsi, %1

    ; finds the lenght based on the provided string
    findLength %1
    syscall

%endmacro

%macro endl 0
    ; Prints out a newline only
    mov rax, SYS_write
    mov rdi, STDOUT
    mov rsi, nlMessage
    mov rdx, 1
    syscall
%endmacro

%macro pushArgs 0
    push rdi
    push rsi
    push rdx
    push rcx
    push r8
    push r9
%endmacro

%macro popArgs 0
    pop r9
    pop r8
    pop rcx
    pop rdx
    pop rsi
    pop rdi
%endmacro

section .data
TRUE equ 1
FALSE equ 0
NULL equ 0
LF equ 10
NEWLINE equ 10

SYS_read	equ	0			; system call code for read
SYS_write	equ	1			; system call code for write
SYS_open	equ	2			; system call code for file open
SYS_close	equ	3			; system call code for file close
SYS_fork	equ	57			; system call code for fork
SYS_exit	equ	60			; system call code for terminate
SYS_creat	equ	85			; system call code for file open/create
SYS_time	equ	201			; system call code for get time

O_RDONLY	equ	000000q			; file permission - read only
O_WRONLY	equ	000001q			; file permission - write only
O_RDWR		equ	000002q			; file permission - read and write

STDIN		equ	0			; standard input
STDOUT		equ	1			; standard output
STDERR		equ	2			; standard error

O_CREAT		equ	0x40
O_TRUNC		equ	0x200
O_APPEND	equ	0x400

invalidArgumentCount db "You have given an invalid command line argument. Make sure it follows this format:",NEWLINE," ./main -f <fileName> -w <word>", NULL
invalidFArgument db "Your first argument is not -f.", NEWLINE ,NULL
invalidWArgument db "Your second argument is not -w.", NEWLINE, NULL
invalidFile db "You did not open a valid file. Try Again", NEWLINE, NULL
invalidWord db "Your word has exceded the limit of MAXWORDLENGTH", NEWLINE, NULL
nlMessage db NEWLINE, NULL

fileReadFailedMessage db "SYSREAD returned -1", NEWLINE, NULL

BUFFSIZE equ 750000
buffIndex dq BUFFSIZE
buffCurr dq BUFFSIZE
wasEOF db 0

section .bss

buffer resb BUFFSIZE

section .text

; rdi = int argc
; rsi = char* argv[]
; rdx = int MAXWORDLENGTH
; rcx = char[] wordSaved
; r8  = long long& fileDescriptor
global checkParams
checkParams:
;mov qword[r8], NULL ; initialize fileDescriptor to null

; --- check argc == 5 ---
cmp rdi, 5
jne countError

; --- check first argument argv[1] ---
mov r11, qword[rsi + 1*8] ; r11 = argv[1]
cmp byte[r11], 45 ; first char should be -
jne fArgError
cmp byte[r11 + 1], 102 ; second char should be f
jne fArgError
cmp byte[r11 + 2], 0 ; last char should be null
jne fArgError

; --- check second argument argv[3] ---
mov r11, qword[rsi + 3*8] ; r11 = argv[3]
cmp byte[r11], 45 ; first char should be -
jne wArgError
cmp byte[r11 + 1], 119 ; second char should be w
jne wArgError
cmp byte[r11 + 2], 0 ; last char should be null
jne wArgError

; --- check file exists by attempting to open it ---
pushArgs
mov rax, SYS_open
mov rdi, qword[rsi + 2*8] ; rdi = argv[2] (input file)
mov rsi, O_RDONLY
syscall
popArgs
cmp rax, 0
jl fileError
mov qword[r8], rax ; return file descriptor by reference

; --- check word length ---
mov r11, qword[rsi + 4*8] ; r11 = argv[4] (input word)
mov r10, rdx ; MAXWORDLENGTH in r10
findLength r11 ; length in rdx
cmp rdx, r10
jg lengthError ; change to jge if having = MAXWORDLENGTH is not acceptable
mov r10, 0
copyWord:
mov r9b, byte[r11 + r10] ; r9 = argv[4][i]
mov byte[rcx + r10], r9b ; rcx[i] = argv[4][i]
inc r10
cmp r10, rdx
jl copyWord
mov byte[rcx + rdx], NULL

; return true if every test passed
mov rax, TRUE
jmp endCheckParams

fArgError: ; -f not received
cout invalidFArgument
mov rax, FALSE
jmp endCheckParams

wArgError: ; -w not received
cout invalidWArgument
mov rax, FALSE
jmp endCheckParams

countError: ; argc not 5
cout invalidArgumentCount
cout nlMessage
mov rax, FALSE
jmp endCheckParams

lengthError: ; input word too long
cout invalidWord
mov rax, FALSE
jmp endCheckParams

fileError: ; file open failed
cout invalidFile
mov rax, FALSE
jmp endCheckParams

endCheckParams:
ret

; rdi = char[] wordObtained
; rsi = int MAXWORDLENGTH
; rdx = bool& isValid
; rcx = long long& fileDescriptor
global getWord
getWord:

push r12 ; r12 = local counter
push r13 ; r13 = local buffer index
push r14 ; r14b = current character
push r15 ; r15 = buffIndex

; File I/O
; What is REQUIRED for a File I/O? - File Descriptor (Location of where the file is in memory)
; 1) Check if we have reached the End of the File
    cmp byte[wasEOF], TRUE
    ;   1.1) If you have reached the end of the file, skip to step ( 6 )
    je localBuffer
; 2) If you have not reached the end of the file
;   2.1) Check if buff Index == buffCurr
        mov r10, qword[buffIndex]
        cmp r10, qword[buffCurr]
        ; buffIndex -> a global index of my current buffer
        ; it is where your next word starts for you to check
        ; buffCurr how big the buffer is
    ; 2.2) If it is not the same -> then skip to step ( 6 )
        jne localBuffer
; 3) Otherwise, set buff Index = 0
newBuffer:
    mov qword[buffIndex], 0
; 4) do file IO
fileIO:
push rdi
push rsi
push rdx
push rcx
    mov rax, SYS_read
    mov rdi, [rcx]
    mov rsi, buffer
    mov rdx, BUFFSIZE
    syscall
pop rcx
pop rdx
pop rsi
pop rdi

cmp rax, 0
jl readError
; 5) check if rax < BUFFSIZE
    cmp rax, BUFFSIZE
    jge notEOF
    ; 5.1) If it is; set EOF to be true
        mov byte[wasEOF], TRUE
    ; 5.2) Set the buffCurrent to rax
    notEOF:
        mov qword[buffCurr], rax
; 6) Start the local check
    ; you will do whatever checks you need to based on what you are asked
localBuffer:
; 7) set a local counter = 0 (not bufflndex)
    mov r12, 0
; 8) set a local buffer index = 0
    mov r13, 0
; 9) to make life easier -> set a register to buff Index
    mov r15, qword[buffIndex]
; 10) Do the actual checks
    localLoop:
    ; 10.1) Check if the buff Index < buffCurr
        cmp r15, qword[buffCurr]
    ; 10.2) if it is; then get the next word (step 11 )
        jl localGet
    ; 10.3) Else; check if we have reached the end of the file
        cmp byte[wasEOF], TRUE
        jne indexMax
        ; 10.3.1) If it is the EOF, then return false
            mov rax, FALSE
            mov byte[rdx], FALSE ; isValid = false
            jmp endGetWord
        
        indexMax:        ; get a new buffer if index hit max but not EOF
            mov qword[buffIndex], r15
            jmp newBuffer
    ; NOTE: do not forget to print the last word
; 11) The actual get word locally
localGet:
    ; 11.1) Get the character at bufflndex
        mov r14b, byte[buffer + r15]
    ; 11.2) If the character is less than or equal to a space
        cmp r14b, 32
        ; then it means, you have finished getting the next word
        ; so jump to step ( 12? )
            jle wordSave 
    ; 11.3) Save the character into the passed in buffer
        mov byte[rdi + r13], r14b
        ; 11.3.1) Ensure you do not go beyond the limit
            cmp r13, rsi
            jge pastBufferInc
            ; If you do, still get the next char, just do not increment the local buffer index
            inc r13
            ; you should still increment the other two registers (variables)
    ; 11.4) increment local index, counter and buffIndex (the register holding buffIndex)
    pastBufferInc:
        inc r12
        inc r15
    ; loop back to continue getting the word
        jmp localLoop
wordSave:
; 12) Save the actual word
    ; 12.1) increment buffIndex (register) to ensure you start at the next word
        inc r15
    ; 12.2) save the buffIndex back into buffIndex variable
        mov qword[buffIndex], r15
    ; 12.3) Check if the local counter is greater than the max length of the local buffer
        mov byte[rdi + r13], NULL ; null terminate word
        cmp r12, rsi
        jle counterValid
        ; 12.3.1) set invalid to be true, and return true
            mov byte[rdx], FALSE ; isValid = false
            mov rax, TRUE
            jmp endGetWord
            ; else set invalid to be false and return true
            counterValid:
                mov byte[rdx], TRUE ; isValid = true
                mov rax, TRUE
                jmp endGetWord

readError: ; sys read failed
cout fileReadFailedMessage
mov rax, FALSE
mov byte[rdx], FALSE
jmp endGetWord

endGetWord:

pop r15
pop r14
pop r13
pop r12

ret


; rdi = char[] wordObtained
; rsi = char[] wordToCheck
; rdx = int& totalWords
global checkWord
checkWord:
mov r10, 0 ; counter


wordLoop:
mov r11b, byte[rsi + r10] ; command line char
cmp r11b, byte[rdi + r10] ; word obtained char
jne failedCheck ; if not equal, leave
inc r10
cmp r11b, 0 ; if not null, loop
jne wordLoop

; if they are equal and null, stop and increment
inc dword[rdx]
mov rax, TRUE
jmp endCheckWord

failedCheck:
mov rax, FALSE
jmp endCheckWord

endCheckWord:
ret

; rdi = long long fileDescriptor
global closeFile
closeFile:
mov rax, SYS_close
; rdi already has fileDescriptor
syscall
ret

global getLength
getLength:
        mov	rax, 0
    strCountLoop2:
        cmp	byte [rdi+rax], NULL
        je	strCountLoopDone2
        inc	rax
        jmp	strCountLoop2
    strCountLoopDone2:
ret

global	printString
printString:
    ; -----
    ;  Count characters to write.

        mov	rdx, 0
    strCountLoop:
        cmp	byte [rdi+rdx], NULL
        je	strCountLoopDone
        inc	rdx
        jmp	strCountLoop
    strCountLoopDone:
        cmp	rdx, 0
        je	printStringDone

    ; -----
    ;  Call OS to output string.

        mov	rax, SYS_write			; system code for write()
        mov	rsi, rdi			; address of characters to write
        mov	rdi, STDOUT			; file descriptor for standard in
                            ; rdx=count to write, set above
        syscall					; system call

    ; -----
    ;  String printed, return to calling routine.

    printStringDone:
ret