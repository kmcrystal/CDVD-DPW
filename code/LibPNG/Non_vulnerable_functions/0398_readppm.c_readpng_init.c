
int readpng_init(FILE *infile, ulg *pWidth, ulg *pHeight)
{
    static uch ppmline[256];
    int maxval;
    saved_infile = infile;
    fgets(ppmline, 256, infile);
    if (ppmline[0] != 'P' || ppmline[1] != '6') {
        fprintf(stderr, "ERROR:  not a PPM file\n");
        return 1;
    }
    
    if (ppmline[1] == '6') {
        color_type = 2;
        channels = 3;
    } else if (ppmline[1] == '8') {
        color_type = 6;
        channels = 4;
    } else  {
        color_type = 0;
        channels = 1;
    }
    do {
        fgets(ppmline, 256, infile);
    } while (ppmline[0] == '#');
    sscanf(ppmline, "%lu %lu", &width, &height);
    do {
        fgets(ppmline, 256, infile);
    } while (ppmline[0] == '#');
    sscanf(ppmline, "%d", &maxval);
    if (maxval != 255) {
        fprintf(stderr, "ERROR:  maxval = %d\n", maxval);
        return 2;
    }
    bit_depth = 8;
    *pWidth = width;
    *pHeight = height;
    return 0;
}
