  }
static void dump_info(FILE *dumpfile, int format, char *prefix, char *msg, ...)
  {
  if (format == DUMP_TEXT)
    {
    va_list ap;
    va_start(ap, msg);
    fprintf(dumpfile, "%s ", prefix);
    vfprintf(dumpfile, msg, ap);
    fprintf(dumpfile, "\n");
    va_end(ap);
    }
  }
