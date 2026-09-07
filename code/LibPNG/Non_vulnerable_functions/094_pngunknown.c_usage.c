static PNG_NORETURN void
usage(const char *program, const char *reason)
{
   fprintf(stderr, "pngunknown: %s: usage:\n %s [--strict] "
      "--default|{(CHNK|default|all)=(default|discard|if-safe|save)} "
      "testfile.png\n", reason, program);
   exit(99);
}
