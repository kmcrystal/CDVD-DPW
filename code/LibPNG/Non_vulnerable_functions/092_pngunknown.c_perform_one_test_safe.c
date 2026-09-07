static void
perform_one_test_safe(FILE *fp, int argc, const char **argv,
   png_uint_32 *default_flags, display *d, const char *test)
{
   if (setjmp(d->error_return) == 0)
   {
      d->test = test; 
#     ifdef PNG_SAVE_UNKNOWN_CHUNKS_SUPPORTED
         perform_one_test(fp, argc, argv, default_flags, d, 0);
#     endif
#     ifdef PNG_READ_USER_CHUNKS_SUPPORTED
         perform_one_test(fp, argc, argv, default_flags, d, 1);
#     endif
      d->test = init; 
   }
}
