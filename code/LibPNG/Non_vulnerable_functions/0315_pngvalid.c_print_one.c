static void
print_one(const char *leader, double err)
{
   if (err != -1.)
      printf(" %s %.5f\n", leader, err);
}
