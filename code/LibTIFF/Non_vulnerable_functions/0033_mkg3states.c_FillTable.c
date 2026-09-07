static void
FillTable(TIFFFaxTabEnt *T, int Size, struct proto *P, int State)
{
    int limit = 1 << Size;
    while (P->val) {
	int width = P->val & 15;
	int param = P->val >> 4;
	int incr = 1 << width;
	int code;
	for (code = P->code; code < limit; code += incr) {
	    TIFFFaxTabEnt *E = T+code;
	    E->State = State;
	    E->Width = width;
	    E->Param = param;
	}
	P++;
    }
}
