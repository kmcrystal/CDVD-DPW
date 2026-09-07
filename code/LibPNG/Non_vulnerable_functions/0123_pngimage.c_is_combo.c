static int
is_combo(int transforms)
{
   return transforms & (transforms-1); 
}
