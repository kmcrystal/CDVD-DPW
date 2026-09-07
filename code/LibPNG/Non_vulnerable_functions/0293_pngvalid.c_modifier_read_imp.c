static void
modifier_read_imp(png_modifier *pm, png_bytep pb, png_size_t st)
{
   while (st > 0)
   {
      size_t cb;
      png_uint_32 len, chunk;
      png_modification *mod;
      if (pm->buffer_position >= pm->buffer_count) switch (pm->state)
      {
         static png_byte sign[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };
         case modifier_start:
            store_read_imp(&pm->this, pm->buffer, 8); 
            pm->buffer_count = 8;
            pm->buffer_position = 0;
            if (memcmp(pm->buffer, sign, 8) != 0)
               png_error(pm->this.pread, "invalid PNG file signature");
            pm->state = modifier_signature;
            break;
         case modifier_signature:
            store_read_imp(&pm->this, pm->buffer, 13+12); 
            pm->buffer_count = 13+12;
            pm->buffer_position = 0;
            if (png_get_uint_32(pm->buffer) != 13 ||
                png_get_uint_32(pm->buffer+4) != CHUNK_IHDR)
               png_error(pm->this.pread, "invalid IHDR");
            
            mod = pm->modifications;
            while (mod != NULL)
            {
               if (mod->chunk == CHUNK_IHDR && mod->modify_fn &&
                   (*mod->modify_fn)(pm, mod, 0))
                  {
                  mod->modified = 1;
                  modifier_setbuffer(pm);
                  }
               
               mod = mod->next;
            }
            
            pm->bit_depth = pm->buffer[8+8];
            pm->colour_type = pm->buffer[8+8+1];
            pm->state = modifier_IHDR;
            pm->flush = 0;
            break;
         case modifier_IHDR:
         default:
            



            if ((cb = pm->flush) > 0)
            {
               if (cb > st) cb = st;
               pm->flush -= cb;
               store_read_imp(&pm->this, pb, cb);
               pb += cb;
               st -= cb;
               if (st == 0) return;
            }
            


            if (pm->pending_chunk != 0)
            {
               png_save_uint_32(pm->buffer, pm->pending_len);
               png_save_uint_32(pm->buffer+4, pm->pending_chunk);
               pm->pending_len = 0;
               pm->pending_chunk = 0;
            }
            else
               store_read_imp(&pm->this, pm->buffer, 8);
            pm->buffer_count = 8;
            pm->buffer_position = 0;
            
            len = png_get_uint_32(pm->buffer);
            chunk = png_get_uint_32(pm->buffer+4);
            


            if (chunk == CHUNK_PLTE || chunk == CHUNK_IDAT ||
                chunk == CHUNK_IEND)
            {
               mod = pm->modifications;
               while (mod != NULL)
               {
                  if ((mod->add == chunk ||
                      (mod->add == CHUNK_PLTE && chunk == CHUNK_IDAT)) &&
                      mod->modify_fn != NULL && !mod->modified && !mod->added)
                  {
                     


                     mod->added = 1;
                     if ((*mod->modify_fn)(pm, mod, 1 ))
                     {
                        
                        if (pm->buffer_count > 0)
                           modifier_setbuffer(pm);
                        else
                           {
                           pm->buffer_position = 0;
                           mod->removed = 1;
                           }
                        


                        pm->pending_len = len;
                        pm->pending_chunk = chunk;
                        break; 
                     }
                  }
                  mod = mod->next;
               }
               



               if (mod != NULL)
                  break; 
            }
            



            if (len+12 <= sizeof pm->buffer)
            {
               store_read_imp(&pm->this, pm->buffer+pm->buffer_count,
                   len+12-pm->buffer_count);
               pm->buffer_count = len+12;
               
               mod = pm->modifications;
               while (mod != NULL)
               {
                  if (mod->chunk == chunk)
                  {
                     if (mod->modify_fn == NULL)
                     {
                        
                        pm->buffer_count = pm->buffer_position = 0;
                        mod->removed = 1;
                        break; 
                     }
                     else if ((*mod->modify_fn)(pm, mod, 0))
                     {
                        mod->modified = 1;
                        
                        if (pm->buffer_count == 0)
                        {
                           pm->buffer_position = 0;
                           break;
                        }
                        modifier_setbuffer(pm);
                     }
                  }
                  mod = mod->next;
               }
            }
            else
               pm->flush = len+12 - pm->buffer_count; 
            
            break;
      }
      


      cb = pm->buffer_count - pm->buffer_position;
      if (cb > st)
         cb = st;
      memcpy(pb, pm->buffer + pm->buffer_position, cb);
      st -= cb;
      pb += cb;
      pm->buffer_position += cb;
   }
}
