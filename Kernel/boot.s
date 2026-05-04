//IMPORTANTE, GERMAN ACUERDATE DE QUE ESTO ES X86 NO RISC

.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)
//cabecera del multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384    //16 KB de pila 
stack_top:

//entrada al kernel
.section .text
.global _start
.type _start, @function
_start:
    mov $stack_top, %esp  //registro esp al top de la pila
    call kernel_main      //llamada a la funcion principal
    cli                   //deshabilitar interrupciones
1:  hlt                   //detener la CPU hasta la proxima interrupcion
    jmp 1b                //un salto no hay mas