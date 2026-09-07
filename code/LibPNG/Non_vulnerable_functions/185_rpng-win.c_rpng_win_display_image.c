} 
static int rpng_win_display_image()
{
    uch *src, *dest;
    uch r, g, b, a;
    ulg i, row, lastrow;
    RECT rect;
    Trace((stderr, "beginning display loop (image_channels == %d)\n",
      image_channels))
    Trace((stderr, "(width = %ld, rowbytes = %ld, wimage_rowbytes = %d)\n",
      image_width, image_rowbytes, wimage_rowbytes))





    for (lastrow = row = 0;  row < image_height;  ++row) {
        src = image_data + row*image_rowbytes;
        dest = wimage_data + row*wimage_rowbytes;
        if (image_channels == 3) {
            for (i = image_width;  i > 0;  --i) {
                r = *src++;
                g = *src++;
                b = *src++;
                *dest++ = b;
                *dest++ = g;   
                *dest++ = r;
            }
        } else  {
            for (i = image_width;  i > 0;  --i) {
                r = *src++;
                g = *src++;
                b = *src++;
                a = *src++;
                if (a == 255) {
                    *dest++ = b;
                    *dest++ = g;
                    *dest++ = r;
                } else if (a == 0) {
                    *dest++ = bg_blue;
                    *dest++ = bg_green;
                    *dest++ = bg_red;
                } else {
                    



                    alpha_composite(*dest++, b, a, bg_blue);
                    alpha_composite(*dest++, g, a, bg_green);
                    alpha_composite(*dest++, r, a, bg_red);
                }
            }
        }
        
        if (((row+1) & 0xf) == 0) {
            rect.left = 0L;
            rect.top = (LONG)lastrow;
            rect.right = (LONG)image_width;      
            rect.bottom = (LONG)lastrow + 16L;   
            InvalidateRect(global_hwnd, &rect, FALSE);
            UpdateWindow(global_hwnd);     
            lastrow = row + 1;
        }
    }
    Trace((stderr, "calling final image-flush routine\n"))
    if (lastrow < image_height) {
        rect.left = 0L;
        rect.top = (LONG)lastrow;
        rect.right = (LONG)image_width;      
        rect.bottom = (LONG)image_height;    
        InvalidateRect(global_hwnd, &rect, FALSE);
        UpdateWindow(global_hwnd);     
    }





    return 0;
}
