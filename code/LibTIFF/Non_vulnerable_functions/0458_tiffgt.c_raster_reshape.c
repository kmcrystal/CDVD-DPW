static void
raster_reshape(int win_w, int win_h)
{
        GLfloat xratio = (GLfloat)win_w/img.width;
        GLfloat yratio = (GLfloat)win_h/img.height;
        int     ratio = (int)(((xratio > yratio)?xratio:yratio) * 100);
        glPixelZoom(xratio, yratio);
        glViewport(0, 0, win_w, win_h);
        snprintf(title, 1024, "%s [%u] %d%%", filelist[fileindex],
                (unsigned int) TIFFCurrentDirectory(tif), ratio);
        glutSetWindowTitle(title);
}
