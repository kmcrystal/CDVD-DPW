static void ourc2pstr( char* inString )
{
	int	sLen = strlen( inString );
	BlockMoveData( inString, &inString[1], sLen );
	inString[0] = sLen;
}
