.data
message:
  .ascii "Test"
.text
.global _start
_start:
    ldr r0, =message
    svc #0

