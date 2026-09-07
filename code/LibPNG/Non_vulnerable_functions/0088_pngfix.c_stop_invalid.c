PNG_NORETURN static void
stop_invalid(struct file *file, const char *what)
{
   stop(file, INVALID_ERROR_CODE, what);
}
