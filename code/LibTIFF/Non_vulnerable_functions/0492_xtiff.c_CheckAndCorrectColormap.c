void
CheckAndCorrectColormap()
{
    register int i;
    for (i = 0; i < colormapSize; i++)
        if ((redMap[i] > 255) || (greenMap[i] > 255) || (blueMap[i] > 255))
            return;
    for (i = 0; i < colormapSize; i++) {
        redMap[i] = SCALE(redMap[i], 255);
        greenMap[i] = SCALE(greenMap[i], 255);
        blueMap[i] = SCALE(blueMap[i], 255);
    }
    TIFFWarning(fileName, "Assuming 8-bit colormap");
}
