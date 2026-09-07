static void
raster_draw(void)
{
  glDrawPixels(img.width, img.height, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid *) raster);
}
