} 
static void rpng2_win_display_row(ulg row)
{
    uch bg_red   = rpng2_info.bg_red;
    uch bg_green = rpng2_info.bg_green;
    uch bg_blue  = rpng2_info.bg_blue;
    uch *src, *src2=NULL, *dest;
    uch r, g, b, a;
    ulg i;
    static int rows=0;
    static ulg firstrow;





    Trace((stderr, "beginning rpng2_win_display_row()\n"))
    if (rows == 0)
        firstrow = row;   
    ++rows;   





    src = rpng2_info.image_data + row*rpng2_info.rowbytes;
    if (bg_image)
        src2 = bg_data + row*bg_rowbytes;
    dest = wimage_data + row*wimage_rowbytes;
    if (rpng2_info.channels == 3) {
        for (i = rpng2_info.width;  i > 0;  --i) {
            r = *src++;
            g = *src++;
            b = *src++;
            *dest++ = b;
            *dest++ = g;   
            *dest++ = r;
        }
    } else  {
        for (i = rpng2_info.width;  i > 0;  --i) {
            r = *src++;
            g = *src++;
            b = *src++;
            a = *src++;
            if (bg_image) {
                bg_red   = *src2++;
                bg_green = *src2++;
                bg_blue  = *src2++;
            }
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




    if ((rows & 0xf) == 0 || row == rpng2_info.height-1) {
        RECT rect;
        rect.left = 0L;
        rect.top = (LONG)firstrow;
        rect.right = (LONG)rpng2_info.width;       
        rect.bottom = (LONG)row + 1L;              
        InvalidateRect(global_hwnd, &rect, FALSE);
        UpdateWindow(global_hwnd);                 
        rows = 0;
    }
} 
