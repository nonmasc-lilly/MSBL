# MSBL

the Miniature Stack Based Language is a programming language
designed to be implemented on small UEFI operating systems
with a standard library with similar features to BASIC.

The hello world program is as follows:

```
(ext print)
const hello = "Hello World\r\n";
main {
    hello$ (print) .
    0 ret
}
```

in fasm-x86 assembly:

```
format pe64 efi
extrn print

section ".text" executable

main:
    mov rax, hello
    push rax
    call print
    pop rax
    mov rax, 0
    return

section ".data" writeable

hello: du "Hello World", 0xD, 0xA, 0x0
```
