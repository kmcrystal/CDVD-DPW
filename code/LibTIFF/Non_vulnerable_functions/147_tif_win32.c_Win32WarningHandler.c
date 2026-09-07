static void
Win32WarningHandler(const char* module, const char* fmt, va_list ap)
{
	LPTSTR szTitle;
	LPTSTR szTmp;
	LPCTSTR szTitleText = "%s Warning";
	LPCTSTR szDefaultModule = "LIBTIFF";
	LPCTSTR szTmpModule = (module == NULL) ? szDefaultModule : module;
	if ((szTitle = (LPTSTR)LocalAlloc(LMEM_FIXED, (strlen(szTmpModule) +
		strlen(szTitleText) + strlen(fmt) + 128)*sizeof(char))) == NULL)
		return;
	sprintf(szTitle, szTitleText, szTmpModule);
	szTmp = szTitle + (strlen(szTitle)+2)*sizeof(char);
	vsprintf(szTmp, fmt, ap);
	MessageBoxA(GetFocus(), szTmp, szTitle, MB_OK | MB_ICONINFORMATION);
	LocalFree(szTitle);
	return;
	if (module != NULL)
		fprintf(stderr, "%s: ", module);
	fprintf(stderr, "Warning, ");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, ".\n");
}
