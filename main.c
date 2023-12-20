#include "ppm_writer.h" 
#include "types.h" 
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    image_t img = image_new();
    write_ppm(img);
    free(img);
    printf("done\n");

    return 0;
}
