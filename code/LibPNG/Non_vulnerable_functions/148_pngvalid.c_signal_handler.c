static png_modifier pm;
static void signal_handler(int signum)
{
   size_t pos = 0;
   char msg[64];
   pos = safecat(msg, sizeof msg, pos, "caught signal: ");
   switch (signum)
   {
      case SIGABRT:
         pos = safecat(msg, sizeof msg, pos, "abort");
         break;
      case SIGFPE:
         pos = safecat(msg, sizeof msg, pos, "floating point exception");
         break;
      case SIGILL:
         pos = safecat(msg, sizeof msg, pos, "illegal instruction");
         break;
      case SIGINT:
         pos = safecat(msg, sizeof msg, pos, "interrupt");
         break;
      case SIGSEGV:
         pos = safecat(msg, sizeof msg, pos, "invalid memory access");
         break;
      case SIGTERM:
         pos = safecat(msg, sizeof msg, pos, "termination request");
         break;
      default:
         pos = safecat(msg, sizeof msg, pos, "unknown ");
         pos = safecatn(msg, sizeof msg, pos, signum);
         break;
   }
   store_log(&pm.this, NULL, msg, 1);
   


   if (signum != SIGTERM)
   {
      struct exception_context *the_exception_context =
         &pm.this.exception_context;
      Throw &pm.this;
   }
   else
      exit(1);
}
