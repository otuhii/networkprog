.data
message: .ascii "Test"
.text
.global _start
_start:
    ldr r0, =message
    ldr r1, =0x1234ABCD
    svc #0

