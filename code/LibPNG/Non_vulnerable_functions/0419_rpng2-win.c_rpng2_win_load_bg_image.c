} 
static int rpng2_win_load_bg_image()
{
    uch *src, *dest;
    uch r1, r2, g1, g2, b1, b2;
    uch r1_inv, r2_inv, g1_inv, g2_inv, b1_inv, b2_inv;
    int k, hmax, max;
    int xidx, yidx, yidx_max = (bgscale-1);
    int even_odd_vert, even_odd_horiz, even_odd;
    int invert_gradient2 = (bg[pat].type & 0x08);
    int invert_column;
    ulg i, row;




    bg_rowbytes = 3 * rpng2_info.width;
    bg_data = (uch *)malloc(bg_rowbytes * rpng2_info.height);
    if (!bg_data) {
        fprintf(stderr, PROGNAME
          ":  unable to allocate memory for background image\n");
        bg_image = 0;
        return 1;
    }




    if ((bg[pat].type & 0x07) == 0) {
        uch r1_min  = rgb[bg[pat].rgb1_min].r;
        uch g1_min  = rgb[bg[pat].rgb1_min].g;
        uch b1_min  = rgb[bg[pat].rgb1_min].b;
        uch r2_min  = rgb[bg[pat].rgb2_min].r;
        uch g2_min  = rgb[bg[pat].rgb2_min].g;
        uch b2_min  = rgb[bg[pat].rgb2_min].b;
        int r1_diff = rgb[bg[pat].rgb1_max].r - r1_min;
        int g1_diff = rgb[bg[pat].rgb1_max].g - g1_min;
        int b1_diff = rgb[bg[pat].rgb1_max].b - b1_min;
        int r2_diff = rgb[bg[pat].rgb2_max].r - r2_min;
        int g2_diff = rgb[bg[pat].rgb2_max].g - g2_min;
        int b2_diff = rgb[bg[pat].rgb2_max].b - b2_min;
        for (row = 0;  row < rpng2_info.height;  ++row) {
            yidx = row % bgscale;
            even_odd_vert = (row / bgscale) & 1;
            r1 = r1_min + (r1_diff * yidx) / yidx_max;
            g1 = g1_min + (g1_diff * yidx) / yidx_max;
            b1 = b1_min + (b1_diff * yidx) / yidx_max;
            r1_inv = r1_min + (r1_diff * (yidx_max-yidx)) / yidx_max;
            g1_inv = g1_min + (g1_diff * (yidx_max-yidx)) / yidx_max;
            b1_inv = b1_min + (b1_diff * (yidx_max-yidx)) / yidx_max;
            r2 = r2_min + (r2_diff * yidx) / yidx_max;
            g2 = g2_min + (g2_diff * yidx) / yidx_max;
            b2 = b2_min + (b2_diff * yidx) / yidx_max;
            r2_inv = r2_min + (r2_diff * (yidx_max-yidx)) / yidx_max;
            g2_inv = g2_min + (g2_diff * (yidx_max-yidx)) / yidx_max;
            b2_inv = b2_min + (b2_diff * (yidx_max-yidx)) / yidx_max;
            dest = bg_data + row*bg_rowbytes;
            for (i = 0;  i < rpng2_info.width;  ++i) {
                even_odd_horiz = (i / bgscale) & 1;
                even_odd = even_odd_vert ^ even_odd_horiz;
                invert_column =
                  (even_odd_horiz && (bg[pat].type & 0x10));
                if (even_odd == 0) {         
                    if (invert_column) {
                        *dest++ = r1_inv;
                        *dest++ = g1_inv;
                        *dest++ = b1_inv;
                    } else {
                        *dest++ = r1;
                        *dest++ = g1;
                        *dest++ = b1;
                    }
                } else {                     
                    if ((invert_column && invert_gradient2) ||
                        (!invert_column && !invert_gradient2))
                    {
                        *dest++ = r2;        
                        *dest++ = g2;        
                        *dest++ = b2;
                    } else {
                        *dest++ = r2_inv;
                        *dest++ = g2_inv;    
                        *dest++ = b2_inv;
                    }
                }
            }
        }




    } else if ((bg[pat].type & 0x07) == 1) {
        hmax = (bgscale-1)/2;   
        max = 2*hmax;           
        r1 = rgb[bg[pat].rgb1_max].r;
        g1 = rgb[bg[pat].rgb1_max].g;
        b1 = rgb[bg[pat].rgb1_max].b;
        r2 = rgb[bg[pat].rgb2_max].r;
        g2 = rgb[bg[pat].rgb2_max].g;
        b2 = rgb[bg[pat].rgb2_max].b;
        for (row = 0;  row < rpng2_info.height;  ++row) {
            yidx = row % bgscale;
            if (yidx > hmax)
                yidx = bgscale-1 - yidx;
            dest = bg_data + row*bg_rowbytes;
            for (i = 0;  i < rpng2_info.width;  ++i) {
                xidx = i % bgscale;
                if (xidx > hmax)
                    xidx = bgscale-1 - xidx;
                k = xidx + yidx;
                *dest++ = (k*r1 + (max-k)*r2) / max;
                *dest++ = (k*g1 + (max-k)*g2) / max;
                *dest++ = (k*b1 + (max-k)*b2) / max;
            }
        }





    } else if ((bg[pat].type & 0x07) == 2) {
        uch ch;
        int ii, x, y, hw, hh, grayspot;
        double freq, rotate, saturate, gray, intensity;
        double angle=0.0, aoffset=0.0, maxDist, dist;
        double red=0.0, green=0.0, blue=0.0, hue, s, v, f, p, q, t;
        fprintf(stderr, "%s:  computing radial background...",
          PROGNAME);
        fflush(stderr);
        hh = rpng2_info.height / 2;
        hw = rpng2_info.width / 2;
        






        angle = CLIP(angle, 0.0, 360.0);
        grayspot = CLIP(bg[pat].bg_gray, 1, (hh + hw));
        freq = MAX((double)bg[pat].bg_freq, 0.0);
        saturate = (double)bg[pat].bg_bsat * 0.1;
        rotate = (double)bg[pat].bg_brot * 0.1;
        gray = 0.0;
        intensity = 0.0;
        maxDist = (double)((hw*hw) + (hh*hh));
        for (row = 0;  row < rpng2_info.height;  ++row) {
            y = row - hh;
            dest = bg_data + row*bg_rowbytes;
            for (i = 0;  i < rpng2_info.width;  ++i) {
                x = i - hw;
                angle = (x == 0)? PI_2 : atan((double)y / (double)x);
                gray = (double)MAX(ABS(y), ABS(x)) / grayspot;
                gray = MIN(1.0, gray);
                dist = (double)((x*x) + (y*y)) / maxDist;
                intensity = cos((angle+(rotate*dist*PI)) * freq) *
                  gray * saturate;
                intensity = (MAX(MIN(intensity,1.0),-1.0) + 1.0) * 0.5;
                hue = (angle + PI) * INV_PI_360 + aoffset;
                s = gray * ((double)(ABS(x)+ABS(y)) / (double)(hw + hh));
                s = MIN(MAX(s,0.0), 1.0);
                v = MIN(MAX(intensity,0.0), 1.0);
                if (s == 0.0) {
                    ch = (uch)(v * 255.0);
                    *dest++ = ch;
                    *dest++ = ch;
                    *dest++ = ch;
                } else {
                    if ((hue < 0.0) || (hue >= 360.0))
                        hue -= (((int)(hue / 360.0)) * 360.0);
                    hue /= 60.0;
                    ii = (int)hue;
                    f = hue - (double)ii;
                    p = (1.0 - s) * v;
                    q = (1.0 - (s * f)) * v;
                    t = (1.0 - (s * (1.0 - f))) * v;
                    if      (ii == 0) { red = v; green = t; blue = p; }
                    else if (ii == 1) { red = q; green = v; blue = p; }
                    else if (ii == 2) { red = p; green = v; blue = t; }
                    else if (ii == 3) { red = p; green = q; blue = v; }
                    else if (ii == 4) { red = t; green = p; blue = v; }
                    else if (ii == 5) { red = v; green = p; blue = q; }
                    *dest++ = (uch)(red * 255.0);
                    *dest++ = (uch)(green * 255.0);
                    *dest++ = (uch)(blue * 255.0);
                }
            }
        }
        fprintf(stderr, "done.\n");
        fflush(stderr);
    }




    for (row = 0;  row < rpng2_info.height;  ++row) {
        src = bg_data + row*bg_rowbytes;
        dest = wimage_data + row*wimage_rowbytes;
        for (i = rpng2_info.width;  i > 0;  --i) {
            r1 = *src++;
            g1 = *src++;
            b1 = *src++;
            *dest++ = b1;
            *dest++ = g1;   
            *dest++ = r1;
        }
    }
    return 0;
} 
