section .data
;Constants
WALL        equ 'x'
GOAL        equ 'o'
EMPTY       equ ' '
SEARCHED    equ '.'

DIGITS      db  "0123456789"

NULL        equ 0
LF          equ 10
global NEW_LINE
NEW_LINE    db  LF, LF

TRUE        equ 1
FALSE       equ 0

;Syscall constants for printing
SYS_WRITE   equ 1
STDOUT      equ 1

;First maze
LENGTH1 equ 5
HEIGHT1 equ 5
maze1   db  "xxxxx",
        db  "x xox",
        db  "x x x",
        db  "x   x",
        db  "xxxxx"
START_POS_X_1 equ   1
START_POS_Y_1 equ   1

;Second maze
LENGTH2 equ 8
HEIGHT2 equ 10
maze2   db  "xxxxxxxx",
        db  "     x  ",
        db  "xxxx x x",
        db  "x  x x x",
        db  "xo x x x",
        db  "x  x x x",
        db  "x  xxx x",
        db  "x      x",
        db  "x      x",
        db  "xxxxxxxx"

START_POS_X_2   equ 4
START_POS_Y_2   equ 5

;Third maze (not findable)
LENGTH3 equ 8
HEIGHT3 equ 10
maze3   db  "xxxxxxxx",
        db  "        ",
        db  "xxxx x x",
        db  "x  x x x",
        db  "xo x x x",
        db  "x  x x x",
        db  "xxxxxx x",
        db  "x      x",
        db  "x      x",
        db  "xxxxxxxx"

START_POS_X_3   equ 4
START_POS_Y_3   equ 5

;Exit syscall
SYS_exit equ 60
EXIT_SUCCESS equ 0

section .text

;Wraps an (x, y) position around the sides of the maze and to the other side
;%1 = x     [32-bit]
;%2 = y     [32-bit]
;%3 = width [32-bit]
;%4 = height[32-bit]
;Wrapped positions placed in x and y arguments, overwriting non-wrapped ones
;This is an optimized version that does not work if x <= -WIDTH or y <= -HEIGHT
%macro wrapAround 4
    ;Wrap around x
    mov eax, %1
    add eax, %3
    mov edx, 0
    div %3
    mov %1, edx

    ;Wrap around y
    mov eax, %2
    add eax, %4
    mov edx, 0
    div %4
    mov %2, edx
%endmacro

;Macro for the isGoalReachable function to see current position.
%macro debugSeeCurrentPos 0
    ;Print a message about the searching position
    mov rax, SYS_WRITE  ;Prepare to write
    mov rdi, STDOUT     ;Write to stdout (default is terminal)
    mov rsi, SEARCH_MSG   ;Move string reference to rsi
    mov rdx, SEARCH_MSG_LEN
    syscall

    ;Print x
    mov rdi, r12
    call printInteger

    mov rax, SYS_WRITE  ;Prepare to write
    mov rdi, STDOUT     ;Write to stdout (default is terminal)
    mov rsi, COMMA_SPACE;Move string reference to rsi
    mov rdx, COMMA_SPCAE_LEN
    syscall

    ;Print y
    mov rdi, r13
    call printInteger

    mov rax, SYS_WRITE  ;Prepare to write
    mov rdi, STDOUT     ;Write to stdout (default is terminal)
    mov rsi, PAREN_ENDL;Move string reference to rsi
    mov rdx, PAREN_ENDL_LEN
    syscall

%endmacro

;Prints a message with the character at the current spot in the array.
;Takes a single argument, the linearized index
%macro debugSeeCurrentChar 1
    push r12
    mov %1, r12

    mov rax, SYS_WRITE  ;Prepare to write
    mov rdi, STDOUT     ;Write to stdout (default is terminal)
    mov rsi, CHAR_MSG   ;Move string reference to rsi
    mov rdx, CHAR_MSG_LEN
    syscall

    mov rax, SYS_WRITE  ;Prepare to write
    mov rdi, STDOUT     ;Write to stdout (default is terminal)
    mov rsi, [rbx + r12] ;Move string reference to rsi
    mov rdx, 1
    syscall

    mov rax, SYS_WRITE  ;Prepare to write
    mov rdi, STDOUT     ;Write to stdout (default is terminal)
    mov rsi, NEW_LINE   ;Move string reference to rsi
    mov rdx, 1
    syscall

    pop r12
%endmacro

;Function labels
global _start
global isGoalReachable
global int2String
extern printInteger ;extern means it's in another file (you can still call it like normal)
extern printMaze 
extern solveMaze
extern convertToLinear

;----------------------- Program starts here -----------------------
_start:

;******** Test int2String ********
mov edi, 4
call printInteger
mov rax, SYS_WRITE  ;Prepare to write
mov rdi, STDOUT     ;Write to stdout (default is terminal)
mov rsi, NEW_LINE
mov rdx, 1
syscall

mov edi, 10
call printInteger
mov rax, SYS_WRITE  ;Prepare to write
mov rdi, STDOUT     ;Write to stdout (default is terminal)
mov rsi, NEW_LINE
mov rdx, 1
syscall

mov edi, 368
call printInteger
mov rax, SYS_WRITE  ;Prepare to write
mov rdi, STDOUT     ;Write to stdout (default is terminal)
mov rsi, NEW_LINE
mov rdx, 2
syscall

afterInt2String:    ;Exists for debugging

;******** Test first maze ********
mov rdi, maze1
mov rsi, START_POS_X_1
mov rdx, START_POS_Y_1
mov rcx, LENGTH1
mov r8, HEIGHT1
call solveMaze

afterFirstMaze:     ;Exists for debugging

;******** Test second maze ********
mov rdi, maze2
mov rsi, START_POS_X_2
mov rdx, START_POS_Y_2
mov rcx, HEIGHT2
mov r8, LENGTH2
call solveMaze

afterSecondMaze:    ;Exists for debugging

;******** Test third maze ********
mov rdi, maze3
mov rsi, START_POS_X_3
mov rdx, START_POS_Y_3
mov rcx, HEIGHT3
mov r8, LENGTH3
call solveMaze

afterThirdMaze:     ;Exists for debugging

;Exiting the program
last:
    mov eax, SYS_exit  ;Load the syscall code for terminating the program
    mov edi, EXIT_SUCCESS ;Load the exit value for the program into rdi
    syscall

;Converts the passed integer to a string
;void int2String(int n, string& str)
;edi = n (the integer to convert to a string)
;rsi = Reference to string to store result. Already allocated.
;edx = Length of the string
int2String:
    ;######### convert an integer to a string in decimal
    
    mov eax, edi
    mov ecx, edx ; this will be i
    dec ecx
    strLoop:

    mov edx, 0 ; prepare to div by 10
    mov r11d, 10
    div r11d
    mov r10d, edx ; this is the last digit


    mov r11b, byte[DIGITS + r10d] ; extract the digit from the const string
    mov byte[rsi + rcx], r11b ; str[i] = digit char
    dec ecx ; this will be i
    
    cmp ecx, 0

    jge strLoop


    ret

;Determines if the maze can be solved starting from the given x, y position
;bool isGoalReachable(char maze[], int start_x, int start_y, int height, int width)
;rdi = Reference to maze. Maze is 2D, although it is linearized.
;esi = start_x
;edx = start_y
;ecx = height
;r8d = width
;rax = Did the position lead to the goal?

isGoalReachable:

    ;Prologue
    ; push saved registers
    push rbx
    push r12
    push r13
    push r14
    push r15

    
    ;Preserve original arguments. Needed because other function calls may overwrite the registers they are in. 
    ;Saved registers *should* be the same before and after a functiona call by SCC.
    mov rbx, rdi ;Reference in rbx
    mov r12, rsi ;x in r15
    mov r13, rdx ;y in r12
    mov r14, rcx ;height in r14
    mov r15, r8  ;width in r15

    ; now in each recursive call, the callee saved registers are saved on the stack
    ; but we may use them freely without worry they will be overwritten

;wrapAround(&start_x, &start_y, width, height); //Wrap around coordinates to other side of maze if applicable
    ;Wrap around any positions to the other side of the array
    wrapAround r12d, r13d, r15d, r14d
    ; no need to worry about out of bounds location, plus we cant infinite wrap due to searched property

    ;########## TODO: Finish the code to tell if the maze can be solved.
    ; prep for convertToLinear(edi = width, esi = x, edx = y)
    mov edi, r15d
    mov esi, r12d
    mov edx, r13d
    ;int linearizedIndex = convertToLinear(width, start_x, start_y);
    call convertToLinear
    mov r10, rax ; r10 hold linearizedIndex
    ;//Base case
    ;if(maze[linearizedIndex] == GOAL) //If goal, return true
    ;   return true;
    ; get maze[linearizedIndex]
    mov r11b, byte[rbx + r10]
    cmp r11b, GOAL
    je retTrue

    ;else if(maze[linearizedIndex == WALL) //If this is a wall, return to the last space and return false
    ;   return false;
    cmp r11b, WALL
    je retFalse

    ;else if(maze[linearizedIndex == SEARCHED) //If this space has been searched already, return to last space and return false
    ;   return false;
    cmp r11b, SEARCHED
    je retFalse

    ;maze[linearizedIndex] = SEARCHED; //Mark this spot as searched
    mov byte[rbx + r10], SEARCHED

    ;//Otherwise, search adjacent spaces
    ;bool foundGoal = isGoalReachable(nextX, nextY);
    ;if(foundGoal) return true;
    ; left
    mov rsi, r12
    sub rsi, 1      ; x - 1
    mov rdx, r13    ; y
    mov rcx, r14
    mov r8, r15
    mov rdi, rbx
    call isGoalReachable
    cmp rax, TRUE
    je retTrue

    ;Proceed to do the other three directions
    ; up
    mov rsi, r12    ; x
    mov rdx, r13    
    sub rdx, 1      ; y - 1
    mov rcx, r14
    mov r8, r15
    mov rdi, rbx
    call isGoalReachable
    cmp rax, TRUE
    je retTrue

    ; right
    mov rsi, r12
    add rsi, 1      ; x + 1
    mov rdx, r13    ; y
    mov rcx, r14
    mov r8, r15
    mov rdi, rbx
    call isGoalReachable
    cmp rax, TRUE
    je retTrue

    ; down
    mov rsi, r12    ; x
    mov rdx, r13    
    add rdx, 1      ; y + 1
    mov rcx, r14
    mov r8, r15
    mov rdi, rbx
    call isGoalReachable
    cmp rax, TRUE
    je retTrue

    ;return false; //Return false if all four directions did not yield a path to the goal

    retFalse:
    mov rax, FALSE
    jmp epilogue
    
    retTrue:
    mov rax, TRUE

    epilogue:
    ; pop saved registers
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx

    ret



