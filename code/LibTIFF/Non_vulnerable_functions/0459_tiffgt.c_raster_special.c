static void
raster_special(int key, int x, int y)
{
        switch (key) {
                case GLUT_KEY_PAGE_UP:          
                    if (TIFFCurrentDirectory(tif) > 0) {
                            if (TIFFSetDirectory(tif,
                                                 TIFFCurrentDirectory(tif)-1)) {
                                    initImage();
                                    setWindowSize();
                        }
                    } else {
                            TIFFRGBAImageEnd(&img);
                            prevImage();
                            initImage();
                            setWindowSize();
                    }
                break;
                case GLUT_KEY_PAGE_DOWN:        
                    if (!TIFFLastDirectory(tif)) {
                            if (TIFFReadDirectory(tif)) {
                                    initImage();
                                    setWindowSize();
                            }
                    } else {
                            TIFFRGBAImageEnd(&img);
                            nextImage();
                            initImage();
                            setWindowSize();
                    }
                break;
                case GLUT_KEY_HOME:             
                        if (TIFFSetDirectory(tif, 0)) {
                                TIFFRGBAImageEnd(&img);
                                initImage();
                                setWindowSize();
                        }
                break;
                case GLUT_KEY_END:              
                        TIFFRGBAImageEnd(&img);
                        while (!TIFFLastDirectory(tif))
                                TIFFReadDirectory(tif);
                        initImage();
                        setWindowSize();
                break;
        }
        glutPostRedisplay();
}
