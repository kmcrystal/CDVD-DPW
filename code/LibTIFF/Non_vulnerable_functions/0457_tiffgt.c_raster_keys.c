static void
raster_keys(unsigned char key, int x, int y)
{
        switch (key) {
                case 'b':                       
                    photo = PHOTOMETRIC_MINISBLACK;
                    initImage();
                    break;
                case 'l':                       
                    order = FILLORDER_LSB2MSB;
                    initImage();
                    break;
                case 'm':                       
                    order = FILLORDER_MSB2LSB;
                    initImage();
                    break;
                case 'w':                       
                    photo = PHOTOMETRIC_MINISWHITE;
                    initImage();
                    break;
                case 'W':                       
                    owarning = TIFFSetWarningHandler(owarning);
                    initImage();
                    break;
                case 'E':                       
                    oerror = TIFFSetErrorHandler(oerror);
                    initImage();
                    break;
                case 'z':                       
                case 'Z':
                    order = order0;
                    photo = photo0;
                    if (owarning == NULL)
                        owarning = TIFFSetWarningHandler(NULL);
                    if (oerror == NULL)
                        oerror = TIFFSetErrorHandler(NULL);
                    initImage();
                    break;
                case 'q':                       
                case '\033':
                    cleanup_and_exit();
        }
        glutPostRedisplay();
}
