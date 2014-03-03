/* -------------------- */
/* --- mandelbrot.c --- */
/* -------------------- */

#include <math.h>

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h> /* isdigit */
#include <string.h> /* memcpy */

#ifdef OPENMP
#include <omp.h>
#endif

#include "def.h"
#include "nrutil.h"

#include "vdef.h"
#include "vnrutil.h"

#include "mutil.h"
#include "simd_macro.h"
#include "mymacro.h"

#include <x86intrin.h> // si compilateur Intel

// --------------------------------------------------
int mandelbrot_scalar(float a, float b, int max_iter)
// --------------------------------------------------
{
    
    // conseil: afficher le contenu des variables dans la boucles *et* valider via excel
    
    int iter = 0;
  
    // COMPLETER ICI
    float x,t,t2,y,z;
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    t = 0.0f;
    t2= 0.0f;
    
    while((t+t2 <= 4.0f) && (iter < max_iter)) {
		
       t  = x*x; 
       t2 = y*y;             
       t = t - t2 + a;       	
       y = 2*x*y + b;
       x = t;
       iter++;
	}
    

    if(iter < max_iter)
       iter--;

       
    return iter;
}
// ------------------------------
void test_mandelbrot_scalar(void)
// ------------------------------
{
    int iter, max_iter = 20;
    float x0, y0;
    float x1, y1;
    
    puts("------------------------------");
    puts("--- test_mandelbrot_scalar ---");
    puts("------------------------------");
    // tests unitaire pour valider
    
    x0 = -0.8; y0 = 0.3; iter = mandelbrot_scalar(x0, y0, max_iter); DEBUG(printf("(%4.2f %4.2f) -> %3d\n", x0, y0, iter));
    x0 = -0.7; y0 = 0.2; iter = mandelbrot_scalar(x0, y0, max_iter); DEBUG(printf("(%4.2f %4.2f) -> %3d\n", x0, y0, iter));
}
// --------------------------------------------------------------
vuint32 mandelbrot_SIMD_F32(vfloat32 a, vfloat32 b, int max_iter)
// --------------------------------------------------------------
{
    // version avec test de sortie en float
    
    vuint32   iter  = _mm_set1_epi32(0);
    vfloat32  fiter = _mm_set_ps(0,0,0,0);

    vfloat32 x,y,t,t2,t3,un,deux,quatre; 
    // COMPLETER ICI
    int test,i = 0;
    
    x      = _mm_set_ps(0,0,0,0);    
    y      = _mm_set_ps(0,0,0,0);
    deux   = _mm_set_ps(2,2,2,2);
    quatre = _mm_set_ps(4,4,4,4);
    un     = _mm_set_ps(1,1,1,1);

	int boucle = 1;

    while(boucle){   
			i+=1;

			t  = _mm_mul_ps(x, x);
            t2 = _mm_mul_ps(y, y);
            				

            t3  = _mm_sub_ps(t,t2);
			t3   = _mm_add_ps(x,a);
              
			y  = _mm_mul_ps(x,y);
            y  = _mm_mul_ps(y,deux);
			y  = _mm_add_ps(y,b);

			x  = t3; 	

			t3 = _mm_cmple_ps(_mm_mul_ps(t,t2),quatre); 
 			
			t = _mm_andnot_ps(t3,un);
			
			fiter = _mm_add_ps(fiter,t);		
			
			
			if(i>=max_iter) 	
				boucle=0;
  	}
 	
	iter = _mm_cvtps_epi32(fiter);

    return iter;
}
// --------------------------------------------------------------
vuint32 mandelbrot_SIMD_I32(vfloat32 a, vfloat32 b, int max_iter)
// --------------------------------------------------------------
{
    // version avec test de sortie en int
    
    vuint32 iter = _mm_set1_epi32(0);
    
    return iter;
}
// ----------------------------
void test_mandelbrot_SIMD(void)
// ----------------------------
{
    int iter = 20;
    vuint32 i;
    vfloat32 a, b;
    double t0, t1, dt;
    
    puts("----------------------------");
    puts("--- test_mandelbrot_SIMD ---");
    puts("----------------------------");
    
    a = _mm_setr_ps(-0.8, -0.7, -0.8, -0.7);
    b = _mm_setr_ps(+0.3, +0.2, -0.3, -0.2);
        
    iter = 15;
    puts("mandelbrot_SIMD_F32");
    a = _mm_setr_ps(-1.00, -0.90, -0.80, -0.70);
    b = _mm_setr_ps(+0.40, +0.30, +0.30, +0.10);
    i = mandelbrot_SIMD_F32(a, b, iter);
    display_vuint32(i, "%10d ", "iter"); puts("");
    
    puts("mandelbrot_SIMD_I32");
    a = _mm_setr_ps(-1.00, -0.90, -0.80, -0.70);
    b = _mm_setr_ps(+0.40, +0.30, +0.30, +0.10);
    i = mandelbrot_SIMD_I32(a, b, iter);
    display_vuint32(i, "%10d ", "iter"); puts("");
}

// --------------------------------------------------------------------------------------------------------
void calc_mandelbrot_scalar(uint32 **M, int h, int w, float a0, float a1, float b0, float b1, int max_iter)
// --------------------------------------------------------------------------------------------------------
{
    // intervale de valeurs: [a0:a1]x[b0:b1]
    
    // la seule chose a modifier dans cette fonction est la ligne de pragma OpenMP
    
    int i, j;
    
    float da, db;
    float32 a, b;
    uint32 iter;
    
    da = (a1-a0)/w;
    db = (b1-b0)/h;
    
#ifdef OPENMP
// COMPLETER ICI
#pragma omp parallel for schedule(dynamic)
#endif   
    
    for(i=0; i<h; i++) {
        for(j=0; j<w; j++) {
            
            // conversion (i,j) -> (x,y)
            a = a0 + i * da;
            b = b0 + j * db;
            
            iter = mandelbrot_scalar(a, b, max_iter);
            
            M[i][j] = iter;
        }
    }
}
// -----------------------------------------------------------------------------------------------------------
void calc_mandelbrot_SIMD_F32(vuint32 **M, int h, int w, float a0, float a1, float b0, float b1, int max_iter)
// -----------------------------------------------------------------------------------------------------------
{
    int i, j;
    
    float da, db;
    float sa, sb;
    vfloat32 a, b;
    vuint32 iter;
    
    da = (a1-a0)/w;
    db = (b1-b0)/h;
    

#ifdef OPENMP
    // COMPLETER ICI
#endif   

    for(i=0; i<h; i++) {
        for(j=0; j<w/4; j++) {
            
            // conversion (i,j) -> (x,y)
            sa = a0 + i * da;
            sb = b0 + j * db * 4;
            
            a = _mm_setr_ps(sa, sa+da, sa+2*da, sa+3*da);
            b = _mm_set1_ps(sb);
            
            iter = mandelbrot_SIMD_F32(a, b, max_iter);
            M[i][j] = iter;
        }
    }
}
// -----------------------------------------------------------------------------------------------------------
void calc_mandelbrot_SIMD_I32(vuint32 **M, int h, int w, float a0, float a1, float b0, float b1, int max_iter)
// -----------------------------------------------------------------------------------------------------------
{
    int i, j;
    
    float da, db;
    float sa, sb;
    vfloat32 a, b;
    vuint32 iter;
    
    da = (a1-a0)/w;
    db = (b1-b0)/h;

#ifdef OPENMP
    // COMPLETER ICI
    
#endif   

    for(i=0; i<h; i++) {
        for(j=0; j<w/4; j++) {
            
            // conversion (i,j) -> (x,y)
            sa = a0 + i * da;
            sb = b0 + j * db * 4;
            
            a = _mm_setr_ps(sa, sa+da, sa+2*da, sa+3*da);
            b = _mm_set1_ps(sb);
            
            iter = mandelbrot_SIMD_I32(a, b, max_iter);
            M[i][j] = iter;
        }
    }
}
// -----------------------------------------------------------------------------------
convert_ui32matrix_ui8matrix(uint32 **m32, int i0, int i1, int j0, int j1, uint8 **m8)
// -----------------------------------------------------------------------------------
{
    int i, j;
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            m8[i][j] = (uint8) m32[i][j];
        }
    }
}
// ----------------------------------------------
void bench_mandelbrot_scalar(int n, int max_iter)
// ----------------------------------------------
{
    // ne rien modifier dans cette fonction
    
    int h, w;
    int i0, i1, j0, j1;
    float a0, a1, b0, b1;
    uint32 **M32;
    uint8 **M8;
    
    
    // chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;
    double cycles;
    
    //puts("-----------------------------");
    //puts("-- bench_mandelbrot_scalar --");
    //puts("-----------------------------");
    
    h = w = n;
    M32 = ui32matrix(0, h-1, 0, w-1);
    M8  = ui8matrix(0, h-1, 0, w-1);
    
    a0 = -1.5; a1 = +0.5;
    b0 = -1.0; b1 = +1.0;
    
    CHRONO(calc_mandelbrot_scalar(M32, h, w, a0, a1, b0, b1, max_iter), cycles);  printf("scalar:   %10.2f\n", cycles/(n*n));
    
    DEBUG(convert_ui32matrix_ui8matrix(M32, 0, h-1, 0, w-1, M8));
    DEBUG(SavePGM_ui8matrix(M8, 0, h-1, 0, w-1, "M_scalar.pgm"));
    
    free_ui32matrix(M32, 0, h-1, 0, w-1);
    free_ui8matrix(M8, 0, h-1, 0, w-1);
}
// --------------------------------------------
void bench_mandelbrot_SIMD(int n, int max_iter)
// --------------------------------------------
{   
    // ne rien modifier dans cette fonction
    
    int h, w;
    float a0, a1, b0, b1;
    vuint32 **M32;
    uint32 **wM32;
    uint8 **M8;
    
    // chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;
    double cycles;
    
    //puts("---------------------------");
    //puts("-- bench_mandelbrot_SIMD --");
    //puts("---------------------------");
    h = w = n;
    
    M32 = vui32matrix(0, h-1, 0, w/4-1);
    M8  = ui8matrix(0, h-1, 0, w-1);
    wM32 = (uint32**) M32;
    
    // ne pas changer
    a0 = -1.5; a1 = +0.5;
    b0 = -1.0; b1 = +1.0;
    
    CHRONO(calc_mandelbrot_SIMD_F32(M32, h, w, a0, a1, b0, b1, max_iter), cycles);
    printf("SIMD F32: %10.2f\n", cycles/(n*n));
    
    CHRONO(calc_mandelbrot_SIMD_I32(M32, h, w, a0, a1, b0, b1, max_iter), cycles); // facultatif
    printf("SIMD I32: %10.2f\n\n", cycles/(n*n));
    
    DEBUG(convert_ui32matrix_ui8matrix(wM32, 0, h-1, 0, w-1, M8));
    DEBUG(SavePGM_ui8matrix(M8, 0, h-1, 0, w-1, "M_v.pgm"));
    
    free_vui32matrix(M32, 0, h-1, 0, w/4-1);
    free_ui8matrix(M8, 0, h-1, 0, w-1);
}

// =========================================
int main_mandelbrot(int argc, char * argv[])
// =========================================
{
    int n, max_iter; // pour avoir les meme param en scalar et SIMD ...
    
    test_mandelbrot_scalar();
    test_mandelbrot_SIMD();
    
    n = 1000; max_iter = 256;
    printf("n = %4d max_iter = %d\n", n, max_iter);
    bench_mandelbrot_scalar(n, max_iter);
    bench_mandelbrot_SIMD(n, max_iter);
    
    return 0;
}