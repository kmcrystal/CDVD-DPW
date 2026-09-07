static uint16
photoArg(const char* arg)
{
        if (strcmp(arg, "miniswhite") == 0)
            return (PHOTOMETRIC_MINISWHITE);
        else if (strcmp(arg, "minisblack") == 0)
            return (PHOTOMETRIC_MINISBLACK);
        else if (strcmp(arg, "rgb") == 0)
            return (PHOTOMETRIC_RGB);
        else if (strcmp(arg, "palette") == 0)
            return (PHOTOMETRIC_PALETTE);
        else if (strcmp(arg, "mask") == 0)
            return (PHOTOMETRIC_MASK);
        else if (strcmp(arg, "separated") == 0)
            return (PHOTOMETRIC_SEPARATED);
        else if (strcmp(arg, "ycbcr") == 0)
            return (PHOTOMETRIC_YCBCR);
        else if (strcmp(arg, "cielab") == 0)
            return (PHOTOMETRIC_CIELAB);
        else if (strcmp(arg, "logl") == 0)
            return (PHOTOMETRIC_LOGL);
        else if (strcmp(arg, "logluv") == 0)
            return (PHOTOMETRIC_LOGLUV);
        else
            return ((uint16) -1);
}
