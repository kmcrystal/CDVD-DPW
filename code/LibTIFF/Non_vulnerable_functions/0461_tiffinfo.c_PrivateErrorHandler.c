static void
PrivateErrorHandler(const char* module, const char* fmt, va_list ap)
{
        if (old_error_handler)
                (*old_error_handler)(module,fmt,ap);
	status = 1;
}
