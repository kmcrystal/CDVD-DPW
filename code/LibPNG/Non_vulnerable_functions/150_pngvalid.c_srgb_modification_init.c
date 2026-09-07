static void
srgb_modification_init(srgb_modification *me, png_modifier *pm, png_byte intent)
{
   modification_init(&me->this);
   me->this.chunk = CHUNK_sBIT;
   if (intent <= 3) 
   {
      me->this.modify_fn = srgb_modify;
      me->this.add = CHUNK_PLTE;
      me->intent = intent;
   }
   else
   {
      me->this.modify_fn = 0;
      me->this.add = 0;
      me->intent = 0;
   }
   me->this.next = pm->modifications;
   pm->modifications = &me->this;
}
