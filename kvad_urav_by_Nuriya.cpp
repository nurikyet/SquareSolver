#include<stdio.h>
#include<math.h>
#include "TXLib.h"
int SolveSquare (double a, double b, double c, double* x1, double* x2);
int Lin(double b, double c);
int doubt(double d);
const double epsilon = 0.0000001;
enum roots
{
    SS_INF_ROOTS = -1,
    zero_root,
    one_root,
    two_roots
};


int main()
  {
  printf("Quadratic equation version - 15555\n" "Type equation a*x**2+b*x+c = 0,");
  printf(" enter odds a, b, c\n");


  double a = 0, b = 0, c = 0;
    if (scanf("%lg %lg %lg", &a, &b, &c) == 3)
      {
        double x1 = 0, x2 = 0;
        int nRoots = SolveSquare(a, b, c, &x1, &x2);
        switch (nRoots)
        {
            case zero_root:  printf("No roots\n");
                break;
            case one_root:  printf("x = %lg\n", x1);
                break;
            case two_roots:  printf("x1 = %lg, x2 = %lg\n", x1, x2);
                break;
            case SS_INF_ROOTS: printf("Infinite number of roots\n");
                break;
            default: printf("main():ERROR: nRoots = %d\n", nRoots);
                return 1;
        }
      }
    else
    printf("Incorrect input");
 }

 int SolveSquare (double a, double b, double c, double* x1, double* x2)

   {
   assert (x1 != NULL);
   assert (x2 != NULL);
   assert (x1 != x2);

   if (a == 0)
     {
     if (b == 0)
       {
       return (c == 0)? SS_INF_ROOTS : zero_root;
       }
     else /* if b != 0 */
       {
       *x1 = Lin(b, c);
       return one_root;
       }
     }
   else /* if a != 0 */
     {
     double d = b*b - (4*a*c);
     if ( doubt(d)< epsilon)
       {
       *x1 = -b / (2*a);
       *x2 = -b / (2*a);
       return one_root;
       }
     else if (d > 0)
       {
       double sqrt_d = sqrt(d);

       *x1 = ( - b - sqrt_d) / (2*a);
       *x2 = ( - b + sqrt_d) / (2*a);

       return two_roots;
       }
     else
       {
       return zero_root;
       }

      }

   }
 int Lin(double b, double c)
   {
   return (-c/b);
   }
 int doubt(double d)
   {
    return abs(d - 0);
   }
