}
int validation_checkfp(int count, int argc, char **argv)
{
   int result;
   checkfp_command command;
   checkfp_control control;
   command.number[0] = 0;
   command.limit = 3;
   command.verbose = verbose;
   command.ctimes = 0;
   command.cmillions = 0;
   command.cinvalid = 0;
   command.cnoaccept = 0;
   while (--argc > 0)
   {
      ++argv;
      if (argc > 1 && strcmp(*argv, "-l") == 0)
      {
         --argc;
         command.limit = atoi(*++argv);
      }
      else
      {
         fprintf(stderr, "unknown argument %s\n", *argv);
         return 1;
      }
   }
   control.cnumber = 0;
   control.check_state = start;
   control.at_start = 1;
   control.cdigits_in_state = 0;
   control.limit = command.limit;
   control.state = 0;
   control.is_negative = 0;
   control.is_zero = 1;
   control.number_was_valid = 0;
   result = check_all_characters(&command, control);
   printf("checkfp: %s: checked %d,%.3d,%.3d,%.3d strings (%d invalid)\n",
      result ? "pass" : "FAIL", command.cmillions / 1000,
      command.cmillions % 1000, command.ctimes / 1000, command.ctimes % 1000,
      command.cinvalid);
   return result;
}
