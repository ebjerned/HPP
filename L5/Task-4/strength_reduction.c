#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
   int i;
   int a = 1, b = 2, c = 3, d = 1;
   float x = 0.1, y = 0.5, z = 0.33;
   printf("%d %d %d %d, %f %f %f\n", a, b, c, d, x, y, z);

   for (i=0; i<50000000; i++)
   {
//      c = d*2;
      c = d << 1;
      b = c*15;
     // a = b/16;
      a = b >> 4;
//      d = b/a;
      d = 16;

      z = 0.33;
      y = 2*z;
//      x = y / 1.33;
	x = y*3 /4;
//      z = x / 1.33;
	z = x*3 /4;
   }
   printf("%d %d %d %d, %f %f %f\n", a, b, c, d, x, y, z);
   return 0;
}
