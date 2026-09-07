static void
sbit_modification_init(sbit_modification *me, png_modifier *pm, png_byte sbit)
{
   modification_init(&me->this);
   me->this.chunk = CHUNK_sBIT;
   me->this.modify_fn = sbit_modify;
   me->this.add = CHUNK_PLTE;
   me->sbit = sbit;
   me->this.next = pm->modifications;
   pm->modifications = &me->this;
}
