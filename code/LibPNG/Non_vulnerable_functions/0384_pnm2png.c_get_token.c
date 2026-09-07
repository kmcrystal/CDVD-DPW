 */
void get_token(FILE *pnm_file, char *token)
{
  int i = 0;
  int ret;
  
  do
  {
    ret = fgetc(pnm_file);
    if (ret == '#') {
      
      do
      {
        ret = fgetc(pnm_file);
      }
      while ((ret != '\n') && (ret != '\r') && (ret != EOF));
    }
    if (ret == EOF) break;
    token[i] = (unsigned char) ret;
  }
  while ((token[i] == '\n') || (token[i] == '\r') || (token[i] == ' '));
  
  do
  {
    ret = fgetc(pnm_file);
    if (ret == EOF) break;
    i++;
    token[i] = (unsigned char) ret;
  }
  while ((token[i] != '\n') && (token[i] != '\r') && (token[i] != ' '));
  token[i] = '\0';
  return;
}
