static int
first_transform(int transforms)
{
   return transforms & -transforms; 
}
