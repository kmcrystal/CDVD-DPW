subtract(32)
static biasFn *lineSubtractFn (unsigned bits)
{
	switch (bits) {
		case  8:  return subtract8;
		case 16:  return subtract16;
		case 32:  return subtract32;
	}
	return NULL;
}
