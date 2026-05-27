section .data

;externs from the other file
extern NEW_LINE
extern isGoalReachable
extern int2String

;Functions in this file
global printInteger
global printMaze
global solveMaze
global convertToLinear

NULL        equ 0
LF          equ 10

TRUE        equ 1
FALSE       equ 0

;Syscall constants for printing
SYS_WRITE   equ 1
STDOUT      equ 1

;Messages for printing
MAZE_MSG        db "Maze before searching", LF, NULL
MAZE_MSG_LEN    equ 22
SUCCESS_MSG     db "Goal found!", LF, NULL
SUCCESS_MSG_LEN equ 12
FAIL_MSG        db "Goal not found...", LF, NULL
FAIL_MSG_LEN    equ 18
START_MSG       db "Starting at pos (", NULL
START_MSG_LEN   equ 17
SEARCH_MSG      db "Searching pos (", NULL
SEARCH_MSG_LEN  equ 15
COMMA_SPACE     db  ", ", NULL
COMMA_SPCAE_LEN equ 2
PAREN_ENDL      db  ")", LF, NULL
PAREN_ENDL_LEN  equ 2
CHAR_MSG       db "Character = ", NULL
CHAR_MSG_LEN   equ 12

section .text

;Prints the maze to standard out. This is used for testing and can help with debugging
;void printMaze(char maze[][], int height, int width)
;rdi = Reference to maze
;esi = Height by value
;edx = Width by value
printMaze:
    ;prologue
    push rbx
    push r15

    ;Save copy of width
    mov r15d, edx   

    ;Allocate space for string onto stack
    mov eax, edx
    mov edx, 0
    mul esi         ;Calculate area of maze
    add eax, esi    ;Add space for linefeeds

    mov rbx, 0
    mov ebx, eax    ;Preserve the string length for deallocation
    sub rsp, rbx    ;Allocate string based on length

    ;Create string
    mov r9, 0      ;Maze string counter
    mov r10, 0     ;Printed string counter
    strLoop:
        mov r8b, byte[rdi + r9]  ;Get char from og maze string
        mov byte[rsp + r10], r8b ;Move to created string
    
        ;Check if at the end of a row (mod length is 0)
        inc r9d
        inc r10
        mov edx, 0
        mov rax, r9
        div r15d
        cmp edx, 0
        jne notEndOfRow
        
        ;If at end of row, move linefeed into next pos
        mov byte[rsp + r10], LF
        inc r10

        notEndOfRow:

        cmp r10, rbx
        jb strLoop

    ;Print completed maze string
    mov rax, SYS_WRITE  ;Prepare to write
    mov rdi, STDOUT     ;Write to stdout (default is terminal)
    mov rsi, rsp        ;Move string reference to rsi
    mov rdx, rbx        ;Move length of string
    syscall

    ;Deallocate string
    add rsp, rbx

    ;epilogue
    pop r15
    pop rbx

    ret

;Prints information about the maze and then attempts to solve
;rdi = Reference to the maze
;esi = start_x
;edx = start_y
;ecx = height
;r8d = width
solveMaze:

    ;Prologue (preserve any saved registers that are used)
    push rbx
    push r15
    push r12
    push r13
    push r14

    ;Preserve copies of the arguments in case they are overwritten by other functions
    mov rbx, rdi
    mov r15, rsi
    mov r12, rdx
    mov r13, rcx
    mov r14, r8

    ;Print the starting message
    mov rax, SYS_WRITE  ;Prepare to write
    mov rdi, STDOUT     ;Write to stdout (default is terminal)
    mov rsi, MAZE_MSG   ;Move string reference to rsi
    mov rdx, MAZE_MSG_LEN ;Move the length of the string to print
    syscall

    ;Print the maze
    mov rdi, rbx    ;First arg is maze ref
    mov rsi, r13    ;Second arg is height
    mov rdx, r14     ;Third arg is width
    call printMaze  ;Go to the printMaze function

    ;Print a message about the starting position
    mov rax, SYS_WRITE  ;Prepare to write
    mov rdi, STDOUT     ;Write to stdout (default is terminal)
    mov rsi, START_MSG   ;Move string reference to rsi
    mov rdx, START_MSG_LEN
    syscall

    mov rdi, r15        ;Transfer copy of x
    call printInteger   ;Print x

    mov rax, SYS_WRITE  ;Prepare to write
    mov rdi, STDOUT     ;Write to stdout (default is terminal)
    mov rsi, COMMA_SPACE   ;Move string reference to rsi
    mov rdx, COMMA_SPCAE_LEN
    syscall

    mov rdi, r12        ;Transfer copy of y
    call printInteger   ;Print y

    mov rax, SYS_WRITE  ;Prepare to write
    mov rdi, STDOUT     ;Write to stdout (default is terminal)
    mov rsi, PAREN_ENDL
    mov rdx, PAREN_ENDL_LEN
    syscall

    ;Check if the maze is solvable
    mov rdi, rbx
    mov rsi, r15
    mov rdx, r12
    mov rcx, r13
    mov r8, r14
    call isGoalReachable

    ;Check the results
    cmp rax, TRUE
    je mazeSolvedSuccessfully
    mov rax, SYS_WRITE  ;Prepare to write
    mov rdi, STDOUT     ;Write to stdout (default is terminal)
    mov rsi, FAIL_MSG
    mov rdx, FAIL_MSG_LEN
    syscall
    jmp solveMazeEpilogue

    mazeSolvedSuccessfully:
    mov rax, SYS_WRITE  ;Prepare to write
    mov rdi, STDOUT     ;Write to stdout (default is terminal)
    mov rsi, SUCCESS_MSG
    mov rdx, SUCCESS_MSG_LEN
    syscall

    solveMazeEpilogue:

    mov rax, SYS_WRITE  ;Prepare to write
    mov rdi, STDOUT     ;Write to stdout (default is terminal)
    mov rsi, NEW_LINE
    mov rdx, 1
    syscall

    ;Epilogue (restore the preserved registers from the stack)
    pop r14
    pop r13
    pop r12
    pop r15
    pop rbx

    ret

;Converts the given 2D x, y coordinates to a linear coordinate within the linearized array
;int convertToLinear(int width, int x, int y)
;edi = width by value
;esi = x by value
;edx = y by value
convertToLinear:
    mov eax, edx    ;Move y value to rax for multiplication
    mul edi         ;Multiply y by width
    add eax, esi    ;Add offset of x value

    ret             ;rax now contains linearized index

;Prints an integer to the screen. 
;Can also be used as a reference on local allocations and prologues & epilogues.
;void printInteger(int num)
;edi = num
printInteger:
    ;prologue (save registers)
    push rbx        ;rbx must be preserved since it is used and is saved in SCC

    ;Count number of digits in string
    mov eax, edi
    mov rbx, 0      ;Counter for number of digits
    mov esi, 10
    logLoop: ;Count by taking int log base 10
        mov edx, 0  ;Zero out edx before dividing
        div esi     ;Divide by 10
        inc ebx     ;Increment count of digits
        cmp eax, 0  ;Check if there are any digits remaining
        ja logLoop

    ;Allocate space for string
    sub rsp, rbx    ;Allocate space for the digits
    dec rsp         ;Allocate an extra byte for null terminator

    ;Convert the int to a string
    mov rsi, rsp        ;Move the string reference into arg register. rdi already has num.
    mov edx, ebx
    call int2String

    ;Place null terminator 
    mov byte[rsp + rbx], NULL

    ;Print
    mov rax, SYS_WRITE  ;Prepare to write
    mov rdi, STDOUT     ;Write to stdout (default is terminal)
    mov rsi, rsp        ;Move string reference to rsi
    mov rdx, rbx
    syscall

    ;Deallocate string
    add rsp, rbx
    inc rsp

    ;epilogue
    pop rbx

    ret