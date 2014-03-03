/* --------------------- */
/* --- filtre1_SSE.h --- */
/* --------------------- */

#ifndef __FILTRE1_SSE_H__
#define __FILTRE1_SSE_H__

#ifdef __cplusplus
#pragma message ("C++")
extern "C" {
#endif

/*
 * Definir ici les macros de calcul de vecteurs non alignes
 * vec_left4_ps(v0,v1)
 * vec_left3_ps(v0,v1)
 * vec_left2_ps(v0,v1)
 * vec_left1_ps(v0,v1)
 * vec_right1_ps(v1,v2)
 * vec_right2_ps(v1,v2)
 * vec_right3_ps(v1,v2)
 * vec_right4_ps(v1,v2)
 */


#define vec_left1_ps(v0, v1)  v0
#define vec_left2_ps(v0, v1)  v0
#define vec_left3_ps(v0, v1)  v0
#define vec_left4_ps(v0, v1)  v0

#define vec_right1_ps(v1, v2) v1
#define vec_right2_ps(v1, v2) v1
#define vec_right3_ps(v1, v2) v1
#define vec_right4_ps(v1, v2) v2

/*
 * Definir ici les macros d'extraction de min
 * vMIN3(x0,x1,x2)
 * vMIN5(x0,x1,x2,x3,x4)
 */

#define vMIN2(x0,x1)         
#define vMIN3(x0,x1,x2)      
#define vMIN4(x0,x1,x2,x3)   
#define vMIN5(x0,x1,x2,x3,x4)


/*
 * ===========================
 * === Liste des fonctions ===
 * ===========================
 */

void test_add_dot_vf32vector(void);
void test_sum_vf32vector(void);
void test_min_vf32vector(void);

void test_filtre1_SSE(void);  
    
#ifdef __cplusplus
}
#endif

#endif /* __FILTRE1_SSE_H__ */
