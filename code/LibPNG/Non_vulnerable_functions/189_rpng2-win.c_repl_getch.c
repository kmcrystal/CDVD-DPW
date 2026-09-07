#include <sys/ioctl.h>
int repl_getch( void )
{
  char ch;
  int fd = fileno(stdin);
  struct termio old_tty, new_tty;
  ioctl(fd, TCGETA, &old_tty);
  new_tty = old_tty;
  new_tty.c_lflag &= ~(ICANON | ECHO | ISIG);
  ioctl(fd, TCSETA, &new_tty);
  fread(&ch, 1, sizeof(ch), stdin);
  ioctl(fd, TCSETA, &old_tty);
  return ch;
}
