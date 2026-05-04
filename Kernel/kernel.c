void kernel_main(void){
    //la memoria de video empieza en 0xB8000, puntero a la direccion (VGA)
    unsigned short* video_memory = (unsigned short*)0xB8000;

    const char* str = "Hola mundo desde mi kernel";

    unsigned char color = 0x0F;

    for (int i = 0; str[i] != '\0'; ++i){
        video_memory[i] = (unsigned short)str[i] | (unsigned short)(color << 8);
    }

}