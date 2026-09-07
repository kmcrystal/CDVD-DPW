static void
image_transform_png_set_rgb_to_gray_set(PNG_CONST image_transform *this,
    transform_display *that, png_structp pp, png_infop pi)
{
   PNG_CONST int error_action = 1; 
#  ifdef PNG_FLOATING_POINT_SUPPORTED
      png_set_rgb_to_gray(pp, error_action, data.red_to_set, data.green_to_set);
#  else
      png_set_rgb_to_gray_fixed(pp, error_action, data.red_to_set,
         data.green_to_set);
#  endif
#  ifdef PNG_READ_cHRM_SUPPORTED
      if (that->pm->current_encoding != 0)
      {
         




#        ifdef PNG_FLOATING_POINT_SUPPORTED
#           define API_function png_get_cHRM_XYZ
#           define API_form "FP"
#           define API_type double
#           define API_cvt(x) (x)
#        else
#           define API_function png_get_cHRM_XYZ_fixed
#           define API_form "fixed"
#           define API_type png_fixed_point
#           define API_cvt(x) ((double)(x)/PNG_FP_1)
#        endif
         API_type rX, gX, bX;
         API_type rY, gY, bY;
         API_type rZ, gZ, bZ;
         if ((API_function(pp, pi, &rX, &rY, &rZ, &gX, &gY, &gZ, &bX, &bY, &bZ)
               & PNG_INFO_cHRM) != 0)
         {
            double maxe;
            PNG_CONST char *el;
            color_encoding e, o;
            


            modifier_current_encoding(that->pm, &o);
            normalize_color_encoding(&o);
            



            if (data.red_to_set == -1 && data.green_to_set == -1 &&
               (fabs(o.red.Y - data.red_coefficient) > DBL_EPSILON ||
               fabs(o.green.Y - data.green_coefficient) > DBL_EPSILON ||
               fabs(o.blue.Y - data.blue_coefficient) > DBL_EPSILON))
               png_error(pp, "internal pngvalid cHRM coefficient error");
            
            e.gamma = o.gamma; 
            e.red.X = API_cvt(rX);
            e.red.Y = API_cvt(rY);
            e.red.Z = API_cvt(rZ);
            e.green.X = API_cvt(gX);
            e.green.Y = API_cvt(gY);
            e.green.Z = API_cvt(gZ);
            e.blue.X = API_cvt(bX);
            e.blue.Y = API_cvt(bY);
            e.blue.Z = API_cvt(bZ);
            


            maxe = 0;
            el = "-"; 
#           define CHECK(col,x)\
            {\
               double err = fabs(o.col.x - e.col.x);\
               if (err > maxe)\
               {\
                  maxe = err;\
                  el = #col "(" #x ")";\
               }\
            }
            CHECK(red,X)
            CHECK(red,Y)
            CHECK(red,Z)
            CHECK(green,X)
            CHECK(green,Y)
            CHECK(green,Z)
            CHECK(blue,X)
            CHECK(blue,Y)
            CHECK(blue,Z)
            



            if (maxe >= 1E-5)
            {
               size_t pos = 0;
               char buffer[256];
               pos = safecat(buffer, sizeof buffer, pos, API_form);
               pos = safecat(buffer, sizeof buffer, pos, " cHRM ");
               pos = safecat(buffer, sizeof buffer, pos, el);
               pos = safecat(buffer, sizeof buffer, pos, " error: ");
               pos = safecatd(buffer, sizeof buffer, pos, maxe, 7);
               pos = safecat(buffer, sizeof buffer, pos, " ");
               
               pos = safecat_color_encoding(buffer, sizeof buffer, pos, &o, 0);
               pos = safecat(buffer, sizeof buffer, pos, " -> ");
               pos = safecat_color_encoding(buffer, sizeof buffer, pos, &e, 0);
               png_error(pp, buffer);
            }
         }
      }
#  endif 
   this->next->set(this->next, that, pp, pi);
}
