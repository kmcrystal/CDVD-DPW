 */
BOOL SearchPngList (
        TCHAR *pFileList, int FileCount, int *pFileIndex,
        PTSTR pstrPrevName, PTSTR pstrNextName)
{
    if (FileCount > 0)
    {
        
        if (pstrPrevName != NULL)
        {
            if (*pFileIndex > 0)
                *pFileIndex -= 1;
            else
                *pFileIndex = FileCount - 1;
            strcpy (pstrPrevName, pFileList + (*pFileIndex * MAX_PATH));
        }
        
        if (pstrNextName != NULL)
        {
            if (*pFileIndex < FileCount - 1)
                *pFileIndex += 1;
            else
                *pFileIndex = 0;
            strcpy (pstrNextName, pFileList + (*pFileIndex * MAX_PATH));
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
 */
BOOL SearchPngList (
        TCHAR *pFileList, int FileCount, int *pFileIndex,
        PTSTR pstrPrevName, PTSTR pstrNextName)
{
    if (FileCount > 0)
    {
        
        if (pstrPrevName != NULL)
        {
            if (*pFileIndex > 0)
                *pFileIndex -= 1;
            else
                *pFileIndex = FileCount - 1;
            strcpy (pstrPrevName, pFileList + (*pFileIndex * MAX_PATH));
        }
        
        if (pstrNextName != NULL)
        {
            if (*pFileIndex < FileCount - 1)
                *pFileIndex += 1;
            else
                *pFileIndex = 0;
            strcpy (pstrNextName, pFileList + (*pFileIndex * MAX_PATH));
        }
        return TRUE;
    }
