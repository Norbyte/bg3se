; The validated hook site consumes only AL; RAX and arithmetic flags are dead.
; The controller pointer is published before the CALL instruction is installed.
EXTERN KeyboardMovementControllerMode:QWORD
EXTERN KeyboardMovementOverride:BYTE
PUBLIC KeyboardMovementModeBridge
.code
KeyboardMovementModeBridge PROC
    mov rax, QWORD PTR KeyboardMovementControllerMode
    mov al, BYTE PTR [rax]
    or al, BYTE PTR KeyboardMovementOverride
    ret
KeyboardMovementModeBridge ENDP
END
