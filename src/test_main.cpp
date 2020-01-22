#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../Lib/def.h"
#include "../Lib/nrio.h"
#include "../Lib/nrarith.h"
#include "../Lib/nralloc.h"

int main() {
    char* filename = const_cast<char*> ("../src/archive500/ppm/2.ppm");
    long nrl2, nrh2, ncl2, nch2;
    rgb8** img = LoadPPM_rgb8matrix(filename, &nrl2, &nrh2, &ncl2, &nch2);

    return 0;
}
