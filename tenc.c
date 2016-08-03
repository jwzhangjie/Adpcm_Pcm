#include "adpcm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, void *argv[]){

    FILE *inf, *outf;
    int i;
    int len ;

    inf =  fopen((char*)argv[1], "rb");
    if (inf == NULL) return -1;

    outf = fopen("./encode.pcm", "wb");
    if (outf == NULL) return -2;

    fseek(inf, 0 ,SEEK_END);
    len = ftell(inf);
    fseek(inf, 0, SEEK_SET);

    /*320 -> 80*/
    short indata[160] = {0,};
    char outdata[80] = {0,};

    adpcm_state_t state;
    memset(&state, 0, sizeof(state));

    int left = len % 320;
    len /= 320;

    int flag = 0;
    for (i = 0; i < len; ++i){
#if 0
        if (random() % 20 != 0){
            fread(indata, 2, 160, inf);
            adpcm_coder(indata, outdata, sizeof(indata), &state);
            fwrite(outdata, 1, 80, outf);
            //continue;
        }
#else
        fread(indata, 2, 160, inf);
        adpcm_coder(indata, outdata, sizeof(indata), &state);

        if (random() % 30 == 1) {printf("!");  continue;}
        fwrite(outdata, 1, 80, outf);
#endif

#if 0
        //if (random() % 20 == 0){
        if (i == 30 || i == 90){
            printf("~");
            memset(&state, 0, sizeof(state));
        }
#endif
        printf(".");
    }

    printf("\n");
    fread(indata, 2, left/2, inf);
    adpcm_coder(indata, outdata, left, &state);
    fwrite(outdata, 1, left/4, outf);

    return 0;
}
