}
static void rpng2_x_finish_display(void)
{
    Trace((stderr, "beginning rpng2_x_finish_display()\n"))
    


    rpng2_info.state = kDone;
    printf(
      "Done.  Press Q, Esc or mouse button 1 (within image window) to quit.\n");
    fflush(stdout);
}
