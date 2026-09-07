void
GetTIFFImage()
{
    int pixel_map[3], red_shift, green_shift, blue_shift;
    char *scan_line, *output_p, *input_p;
    uint32 i, j;
    uint16 s;
    scan_line = (char *) malloc(tfBytesPerRow = TIFFScanlineSize(tfFile));
    MCHECK(scan_line);
    if ((tfImageDepth == 32) || (tfImageDepth == 24)) {
        output_p = imageMemory = (char *)
            malloc(tfImageWidth * tfImageHeight * 4);
        MCHECK(imageMemory);
        


        if (ImageByteOrder(xDisplay) == LSBFirst) { 
            red_shift = pixel_map[0] = xRedMask == 0xFF000000 ? 3
                : (xRedMask == 0xFF0000 ? 2 : (xRedMask == 0xFF00 ? 1 : 0));
            green_shift = pixel_map[1] = xGreenMask == 0xFF000000 ? 3
                : (xGreenMask == 0xFF0000 ? 2 : (xGreenMask == 0xFF00 ? 1 : 0));
            blue_shift = pixel_map[2] = xBlueMask == 0xFF000000 ? 3
                : (xBlueMask == 0xFF0000 ? 2 : (xBlueMask == 0xFF00 ? 1 : 0));
        } else { 
            red_shift = pixel_map[0] = xRedMask == 0xFF000000 ? 0
                : (xRedMask == 0xFF0000 ? 1 : (xRedMask == 0xFF00 ? 2 : 3));
            green_shift = pixel_map[0] = xGreenMask == 0xFF000000 ? 0
                : (xGreenMask == 0xFF0000 ? 1 : (xGreenMask == 0xFF00 ? 2 : 3));
            blue_shift = pixel_map[0] = xBlueMask == 0xFF000000 ? 0
                : (xBlueMask == 0xFF0000 ? 1 : (xBlueMask == 0xFF00 ? 2 : 3));
        }
        if (tfPlanarConfiguration == PLANARCONFIG_CONTIG) {
            for (i = 0; i < tfImageHeight; i++) {
                if (TIFFReadScanline(tfFile, scan_line, i, 0) < 0)
                    break;
                for (input_p = scan_line, j = 0; j < tfImageWidth; j++) {
                    *(output_p + red_shift) = *input_p++;
                    *(output_p + green_shift) = *input_p++;
                    *(output_p + blue_shift) = *input_p++;
                    output_p += 4;
                    if (tfSamplesPerPixel == 4) 
                        input_p++;
                }
            }
        } else {
            for (s = 0; s < tfSamplesPerPixel; s++) {
                if (s == 3)             
                    continue;
                for (i = 0; i < tfImageHeight; i++) {
                    if (TIFFReadScanline(tfFile, scan_line, i, s) < 0)
                        break;
                    input_p = scan_line;
                    output_p = imageMemory + (i*tfImageWidth*4) + pixel_map[s];
                    for (j = 0; j < tfImageWidth; j++, output_p += 4)
                        *output_p = *input_p++;
                }
            }
        }
    } else {
        if (xImageDepth == tfImageDepth) {
            output_p = imageMemory = (char *)
                malloc(tfBytesPerRow * tfImageHeight);
            MCHECK(imageMemory);
            for (i = 0; i < tfImageHeight; i++, output_p += tfBytesPerRow)
                if (TIFFReadScanline(tfFile, output_p, i, 0) < 0)
                    break;
        } else if ((xImageDepth == 8) && (tfImageDepth == 4)) {
            output_p = imageMemory = (char *)
                malloc(tfBytesPerRow * 2 * tfImageHeight + 2);
            MCHECK(imageMemory);
            




            for (i = 0; i < tfImageHeight; i++) {
                if (TIFFReadScanline(tfFile, scan_line, i, 0) < 0)
                    break;
                output_p = &imageMemory[i * tfImageWidth];
                input_p = scan_line;
                for (j = 0; j < tfImageWidth; j += 2, input_p++) {
                    *output_p++ = (*input_p >> 4) + basePixel;
                    *output_p++ = (*input_p & 0xf) + basePixel;
                }
            }
        } else if ((xImageDepth == 8) && (tfImageDepth == 2)) {
            output_p = imageMemory = (char *)
                malloc(tfBytesPerRow * 4 * tfImageHeight + 4);
            MCHECK(imageMemory);
            for (i = 0; i < tfImageHeight; i++) {
                if (TIFFReadScanline(tfFile, scan_line, i, 0) < 0)
                    break;
                output_p = &imageMemory[i * tfImageWidth];
                input_p = scan_line;
                for (j = 0; j < tfImageWidth; j += 4, input_p++) {
                    *output_p++ = (*input_p >> 6) + basePixel;
                    *output_p++ = ((*input_p >> 4) & 3) + basePixel;
                    *output_p++ = ((*input_p >> 2) & 3) + basePixel;
                    *output_p++ = (*input_p & 3) + basePixel;
                }
            }
        } else if ((xImageDepth == 4) && (tfImageDepth == 2)) {
            output_p = imageMemory = (char *)
                malloc(tfBytesPerRow * 2 * tfImageHeight + 2);
            MCHECK(imageMemory);
            for (i = 0; i < tfImageHeight; i++) {
                if (TIFFReadScanline(tfFile, scan_line, i, 0) < 0)
                    break;
                output_p = &imageMemory[i * tfBytesPerRow * 2];
                input_p = scan_line;
                for (j = 0; j < tfImageWidth; j += 4, input_p++) {
                    *output_p++ = (((*input_p>>6) << 4)
                        | ((*input_p >> 4) & 3)) + basePixel;
                    *output_p++ = ((((*input_p>>2) & 3) << 4)
                        | (*input_p & 3)) + basePixel;
                }
            }
        } else {
            fprintf(stderr,
                "xtiff: can't handle %d-bit TIFF file on an %d-bit display\n",
                tfImageDepth, xImageDepth);
            exit(0);
        }
    }
    free(scan_line);
}
