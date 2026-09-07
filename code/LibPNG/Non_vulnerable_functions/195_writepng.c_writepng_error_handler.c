}
static void writepng_error_handler(png_structp png_ptr, png_const_charp msg)
{
    mainprog_info  *mainprog_ptr;
    







    fprintf(stderr, "writepng libpng error: %s\n", msg);
    fflush(stderr);
    mainprog_ptr = png_get_error_ptr(png_ptr);
    if (mainprog_ptr == NULL) {         
        fprintf(stderr,
          "writepng severe error:  jmpbuf not recoverable; terminating.\n");
        fflush(stderr);
        exit(99);
    }
    






    longjmp(mainprog_ptr->jmpbuf, 1);
}
