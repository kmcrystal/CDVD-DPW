static void
emitFont(FILE* fd)
{
    static const char* fontPrologue[] = {
	"/newfont 10 dict def newfont begin /FontType 3 def /FontMatrix [1",
	"0 0 1 0 0] def /FontBBox [0 0 512 1] def /Encoding 256 array def",
	"0 1 31{Encoding exch /255 put}for 120 1 255{Encoding exch /255",
	"put}for Encoding 37 /255 put Encoding 40 /255 put Encoding 41 /255",
	"put Encoding 92 /255 put /count 0 def /ls{Encoding exch count 3",
	"string cvs cvn put /count count 1 add def}def 32 1 36{ls}for",
	"38 1 39{ls}for 42 1 91{ls}for 93 1 99{ls}for /count 100",
	"def 100 1 119{ls}for /CharDict 5 dict def CharDict begin /white",
	"{dup 255 eq{pop}{1 dict begin 100 sub neg 512 exch bitshift",
	"/cw exch def cw 0 0 0 cw 1 setcachedevice end}ifelse}def /black",
	"{dup 255 eq{pop}{1 dict begin 110 sub neg 512 exch bitshift",
	"/cw exch def cw 0 0 0 cw 1 setcachedevice 0 0 moveto cw 0 rlineto",
	"0 1 rlineto cw neg 0 rlineto closepath fill end}ifelse}def /numbuild",
