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

    outf = fopen("./decode.pcm", "wb");
    if (outf == NULL) return -2;

    fseek(inf, 0 ,SEEK_END);
    len = ftell(inf);
    fseek(inf, 0, SEEK_SET);

    /*320 -> 80*/
    char indata[80] = {0,};
    short outdata[160] = {0,};

    adpcm_state_t state;
    memset(&state, 0, sizeof(state));

    int left = len % 80;
    len /= 80;

    for (i = 0; i < len; ++i){

        fread(indata, 1, 80, inf);
        adpcm_decoder(indata, outdata, sizeof(indata), &state);
        fwrite(outdata, 2, 160, outf);
    }

    fread(indata, 1, left, inf);
    adpcm_decoder(indata, outdata, left, &state);
    fwrite(outdata, 2, left*2, outf);

    return 0;
}
