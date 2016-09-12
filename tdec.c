#include "adpcm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



static int32_t nglbXn1 = 0;
static int32_t nglbYn1 = 0;
void HighPassFilter(int16_t* psData, int32_t nLen, int32_t nSign) // nSign is 1 for init
{
    int32_t i = 0;
    int32_t Xn, nTmp;
    int16_t* pY = psData;
    if (!psData || 2>nLen) return;
    if (1 == nSign) {
        nglbXn1 = psData[0]; i = 1; pY = psData + 1; psData[0] = 0;

    }
    for (; i<nLen; i++)
    {
        Xn   = *pY;
        //nTmp = (((Xn - nglbXn1 + nglbYn1)<<4) - nglbYn1) >> 4;
        nTmp = (((Xn - nglbXn1 + nglbYn1) * 16) - nglbYn1) / 16; // the precision is not so good, maybe
        
        *pY  = nTmp;
        nglbXn1 = Xn;
        nglbYn1 = *pY++;

    }

}

int main(int argc, void *argv[]){

    volatile int non = 0;
    if (non) ProfilerStop();

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

    int flag = 1;
    for (i = 0; i < len; ++i){

        fread(indata, 1, 80, inf);
        adpcm_decoder(indata, outdata, sizeof(indata), &state);
        //HighPassFilter(outdata, 160, flag);
        flag = 0;
        fwrite(outdata, 2, 160, outf);
    }

    fread(indata, 1, left, inf);
    adpcm_decoder(indata, outdata, left, &state);
    fwrite(outdata, 2, left*2, outf);

    return 0;
}
