// ------------------------ //
// --- sommes_rapides.c --- //
// ------------------------ //

/*
 * Copyright (c) 2014 Lionel Lacassagne
 * LRI, University Paris Sud, CNRS
 */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

// Numerical Recipes stuff, public domain

#define NR_END 0
#define FREE_ARG void*

// ----------------------------
void nrerror(char error_text[])
// ----------------------------
// Numerical Recipes standard error handler
{
    fprintf(stderr,"Numerical Recipes run-time error...\n");
    fprintf(stderr,"%s\n",error_text);
    fprintf(stderr,"...now exiting to system...\n");
    exit(1);
}

// ----------------------------
float* vector(long nl, long nh)
// ----------------------------
/* allocate a float32 vector with subscript range v[nl..nh] */
{
    float *v;
    
    v=(float *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(float)));
    if (!v) nrerror("allocation failure in vector()");
    if(!v) return NULL;
    return v-nl+NR_END;
}
// -----------------------------------------
void free_vector(float *v, long nl, long nh)
// -----------------------------------------
// free a float vector allocated with vector()
{
    free((FREE_ARG) (v+nl-NR_END));
}
// --------------------------------------------------------------------
void init_vector_param(float *v, int i0, int i1, float x0, float xstep)
// --------------------------------------------------------------------
{
    int i;
    float x = x0;
    
    for(i=i0; i<=i1; i++) {
        v[i] = x;
        x += xstep;
    }
}
// ---------------------------------------
void zero_vector(float *v, int i0, int i1)
// ---------------------------------------
{
    init_vector_param(v, i0, i1, 0.0f, 0.0f);
}
// --------------------------------------------------------------------
void display_vector(float *v,long nl,long nh, char *format, char *name)
// --------------------------------------------------------------------
{
    long i;
    
    if(name != NULL) printf("%s", name);
    
    for(i=nl; i<=nh; i++) {
        printf(format, v[i]);
    }
    putchar('\n');
}
// ========================================================================================================
// === Fonctions de calcul ================================================================================
// ========================================================================================================

// ---------------------------------------------
void sum_k_add(float *X, int n, int k, float *Y)
// ---------------------------------------------
{
    // somme "additionnelle"
	int r = (k - 1)/2;
	int i = 0; 
	for(i = 0; i< n; i++){
		
		float midSom = 0.0;		
		// somme interne
		int ri ;
		for(ri = -r; ri <= r; ri++){
			midSom += X[i + ri];
			//printf("%d", i+ri);
		}
		
		Y[i] = midSom;
	}

}
// ----------------------------------------------
void sum_k_diff(float *X, int n, int k, float *Y)
// ----------------------------------------------
{
    // "sommation differentielle"
	int r = (k - 1)/2;
		int i = 0; 
		float som = 0.0;
	
		// premier element
		int ri ;
		for(ri = -r; ri <= r; ri++){
			som += X[ri];
			//printf("%d", i+ri);
		}
		Y[0] = som;

		// la suite
		for(i = 1; i< n; i++){		
	
			som += X[i+r] - X[i-r-1];

			Y[i] = som;
		}

}
// ---------------------------------------
void int_vector(float *X, int n, float *Y)
// ---------------------------------------
{
    // vecteur integrale ou "+scan" associatif
    // (interdiction de changer le prototype de la fonction)

	Y[0] = X[0];
	int i = 1;	
	
	for(i = 1; i <n; i++){		
		Y[i] = Y[i-1] + X[i];
	}	

}
// -------------------------------------------------------
void sum_k_int(float *X, int n, int k, float *I, float *Y)
// -------------------------------------------------------
{
    // "sommation integrale"
	int_vector(X,n,Y);
}
// ------------------------------------------------
void sum_k_add_u2(float *X, int n, int k, float *Y)
// ------------------------------------------------
{
    // somme "additionnelle" avec loop-unrolling
}
// ------------
void test(void)
// ------------
{
    // debug avec excel
    
    float *X; // entree
    float *Sa; // somme additionnelle
    float *Sd; // somme differentielle
    float *Si; // somme integralle
    float *Su; // somme integralle
    float *I;  // vecteur integralle
    
    int n = 16;
    int k, kmax = 7;
    int r, rmax = kmax/2;
    char *format = "%4.0f";
    
    // alloc
    X  = vector(0-rmax,     n-1+rmax);
    Sa = vector(0,          n-1);
    Sd = vector(0,          n-1);
    Si = vector(0,          n-1);
    Su = vector(0,          n-1);
    I  = vector(0-rmax-1,   n-1);
    
    // zero
    zero_vector(X,  0-rmax, n-1+rmax);
    zero_vector(Sa, 0,   n-1);
    zero_vector(Sd, 0,   n-1);
    zero_vector(Si, 0,   n-1);
    zero_vector(Su, 0,   n-1);
    zero_vector(I,  0,   n-1);
    
    // init
    init_vector_param(X, 0, n-1, 1, 1);
    display_vector(X, 0, n-1, format, "X ");
    
    // calcul
    puts("\nsomme additionnelle");
    printf("%2s :", "X");                              display_vector(X,  0, n-1, format, NULL);
    k = 3; sum_k_add(X, n, k, Sa); printf("%2d :", k); display_vector(Sa, 0, n-1, format, NULL);
    k = 5; sum_k_add(X, n, k, Sa); printf("%2d :", k); display_vector(Sa, 0, n-1, format, NULL);
    k = 7; sum_k_add(X, n, k, Sa); printf("%2d :", k); display_vector(Sa, 0, n-1, format, NULL);
    
    puts("\nsomme differentielle");
    printf("%2s :", "X");                               display_vector(X,  0, n-1, format, NULL);
    k = 3; sum_k_diff(X, n, k, Sd); printf("%2d :", k); display_vector(Sd, 0, n-1, format, NULL);
    k = 5; sum_k_diff(X, n, k, Sd); printf("%2d :", k); display_vector(Sd, 0, n-1, format, NULL);
    k = 7; sum_k_diff(X, n, k, Sd); printf("%2d :", k); display_vector(Sd, 0, n-1, format, NULL);
    
    puts("somme integralle"); // astuce: pour debugguer, commencer par k = 7 ...
    int_vector(X, n, I); // interdiction de changer cette ligne
    printf("%2s :", "I");                                  display_vector(I,  0, n-1, format, NULL);
    k = 3; sum_k_int(X, n, k, I, Si);  printf("%2d :", k); display_vector(Si, 0, n-1, format, NULL);
    k = 5; sum_k_int(X, n, k, I, Si);  printf("%2d :", k); display_vector(Si, 0, n-1, format, NULL);
    k = 7; sum_k_int(X, n, k, I, Si);  printf("%2d :", k); display_vector(Si, 0, n-1, format, NULL);
    
    puts("\nsomme additionnelle avec unrolling");
    printf("%2s :", "X");                                 display_vector(X,  0, n-1, format, NULL);
    k = 3; sum_k_add_u2(X, n, k, Su); printf("%2d :", k); display_vector(Su, 0, n-1, format, NULL);
    k = 5; sum_k_add_u2(X, n, k, Su); printf("%2d :", k); display_vector(Su, 0, n-1, format, NULL);
    k = 7; sum_k_add_u2(X, n, k, Su); printf("%2d :", k); display_vector(Su, 0, n-1, format, NULL);
    
    
    // free
    free_vector(X,  0-rmax,     n-1+rmax);
    free_vector(Sa, 0,          n-1);
    free_vector(Sd, 0,          n-1);
    free_vector(Si, 0,          n-1);
    free_vector(Su, 0,          n-1);
    free_vector(I,  0-rmax-1,   n-1);
}
// -----------------------------
int main(int argc, char *argv[])
// -----------------------------
{
    test();
}
