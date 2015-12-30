

#include "connected.h"

#define _TEST
#ifdef _TEST

#include <functional>
#include <stdlib.h>
#include <stdio.h>



void test_inline_img()
{
    const  char *_img = {
	"              "
	"    *  0  *   "
	"   **  0  *   "
	"    *******   "
	"      *       "
	"   *          "
	"  ***         "
    };
    const unsigned char *img = (const unsigned char *)_img;
    int width = 14, height = 7;

    unsigned char *out_uc = (unsigned char *)malloc(width*height);

    ConnectedComponents cc(30);
    cc.connected(img, out_uc, width, height,
		 std::equal_to<unsigned char>(),
		 false);

    for(int r=0; r<height; ++r) {
	for(int c=0; c<width; ++c)
	    putchar('0'+out_uc[r*width+c]);
	putchar('\n');
    }

    free(out_uc);
}



void test_raw_img(const char *infname, const char *outfname,
		  int width, int height)
{
    unsigned char *img = (unsigned char *)malloc(width*height);

    FILE *fin = fopen(infname, "r");
    fread(img, 1, width*height, fin);

    unsigned int *out = (unsigned int *)malloc(sizeof(*out)*width*height);

    ConnectedComponents cc(30);
    cc.connected(img, out, width, height,
		 std::equal_to<unsigned char>(),
		 constant<bool,true>());

    unsigned char *out_uc = (unsigned char *)malloc(width*height);
    std::copy(out, out+width*height, out_uc);
    FILE *fout = fopen(outfname, "w");
    fwrite(out_uc, 1, width*height, fout);

    fclose(fout); fclose(fin);
    free(img); free(out); free(out_uc);
}


// main()
// {
//     test_inline_img();
//     test_raw_img("img.raw", "out.raw", 321, 241);
// }

#endif // _TEST
