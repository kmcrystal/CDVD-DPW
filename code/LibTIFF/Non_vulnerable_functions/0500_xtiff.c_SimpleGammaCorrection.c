void
SimpleGammaCorrection()
{
    register int i;
    register double i_gamma = 1.0 / appData.gamma;
    for (i = 0; i < colormapSize; i++) {
        if (((tfPhotometricInterpretation == PHOTOMETRIC_MINISWHITE)
            && (i == colormapSize - 1))
            || ((tfPhotometricInterpretation == PHOTOMETRIC_MINISBLACK)
            && (i == 0)))
            redMap[i] = greenMap[i] = blueMap[i] = 0;
        else {
            redMap[i] = ROUND((pow(dRed[i] / 65535.0, i_gamma) * 65535.0));
            greenMap[i] = ROUND((pow(dGreen[i] / 65535.0, i_gamma) * 65535.0));
            blueMap[i] = ROUND((pow(dBlue[i] / 65535.0, i_gamma) * 65535.0));
        }
    }
    free(dRed); free(dGreen); free(dBlue);
}
