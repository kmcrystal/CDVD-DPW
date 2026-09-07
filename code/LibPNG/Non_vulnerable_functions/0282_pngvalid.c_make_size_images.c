static void
make_size_images(png_store *ps)
{
   
   safecat(ps->test, sizeof ps->test, 0, "make size images");
   

   make_size(ps, 0, 0, WRITE_BDHI);
   make_size(ps, 2, 3, WRITE_BDHI);
   make_size(ps, 3, 0, 3 );
   make_size(ps, 4, 3, WRITE_BDHI);
   make_size(ps, 6, 3, WRITE_BDHI);
}
