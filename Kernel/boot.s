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

# Exponemos la función para que tu código en C pueda encontrarla
.global gdt_flush

gdt_flush:
    # 1. Cargamos el puntero (gp) en la CPU
    # En C, le pasamos la dirección de 'gp' como parámetro. 
    # En ensamblador x86 de 32 bits, el primer parámetro está en la pila a 4 bytes del inicio.
    mov 4(%esp), %eax
    lgdt (%eax)

    # 2. Actualizamos los Registros de Datos
    # Nuestro Segmento de Datos es el índice 2 de la GDT.
    # Cada entrada ocupa 8 bytes. 2 * 8 = 16 (que en hexadecimal es 0x10).
    mov $0x10, %ax
    mov %ax, %ds    # Data Segment
    mov %ax, %es    # Extra Segment
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss    # Stack Segment

    # 3. Actualizamos el Registro de Código (CS)
    # Por seguridad, Intel no te deja hacer "mov %ax, %cs". 
    # Te obliga a hacer un "Far Jump" (Salto Lejano) para recargar el código.
    # Nuestro Segmento de Código es el índice 1 (1 * 8 = 8, que es 0x08).
    jmp $0x08, $flush_codigo

flush_codigo:
    # Volvemos a nuestro código en C
    ret