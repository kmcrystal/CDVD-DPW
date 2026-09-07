static void
gama_modification_init(gama_modification *me, png_modifier *pm, double gammad)
{
   double g;
   modification_init(&me->this);
   me->this.chunk = CHUNK_gAMA;
   me->this.modify_fn = gama_modify;
   me->this.add = CHUNK_PLTE;
   g = fix(gammad);
   me->gamma = (png_fixed_point)g;
   me->this.next = pm->modifications;
   pm->modifications = &me->this;
}
