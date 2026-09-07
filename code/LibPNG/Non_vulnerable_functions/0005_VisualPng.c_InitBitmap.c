 */
BOOL InitBitmap (BYTE *pDiData, int cxWinSize, int cyWinSize)
{
    BYTE *dst;
    int x, y, col;
    
    dst = pDiData;
    for (y = 0; y < cyWinSize; y++)
    {
        col = 0;
        for (x = 0; x < cxWinSize; x++)
        {
            
            *dst++ = 127;
            *dst++ = 127;
            *dst++ = 127;
            col += 3;
        }
        
        while ((col % 4) != 0)
        {
            dst++;
            col++;
        }
    }
    return TRUE;
}
