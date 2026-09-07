const unsigned char*
TIFFGetBitRevTable(int reversed)
{
	return (reversed ? TIFFBitRevTable : TIFFNoBitRevTable);
}
