/* -------------- */
/* --- main.c --- */
/* -------------- */

// --- NRC Framework --- //
#include "def.h"
#include "nrutil.h"
//#include "timers_b.h"
// --- vNRC Framework --- //
#include "vdef.h"
#include "vnrutil.h"

#include "mutil.h"

#include "filtre1_SSE.h"

int main(void)
{
    
    test_add_dot_vf32vector();
    test_sum_vf32vector();
    test_min_vf32vector();

    return 0;
    
}