} 
static void rpng2_win_finish_display()
{
    Trace((stderr, "beginning rpng2_win_finish_display()\n"))
    


    rpng2_info.state = kDone;
    printf(
#ifndef __CYGWIN__
      "Done.  Press Q, Esc or mouse button 1 (within image window) to quit.\n"
#else
      "Done.  Press mouse button 1 (within image window) to quit.\n"
#endif
    );
    fflush(stdout);
}
