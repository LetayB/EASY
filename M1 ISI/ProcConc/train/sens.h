#ifndef _SENS_H_
#define _SENS_H_

/* Types */

typedef enum sens_s { OUEST_EST=0, EST_OUEST=1 } sens_t ;

/* Fonctions */

extern void sens_print( sens_t s ) ;
extern void sens_blanc_print() ;
extern void sens_vide_print() ;

#endif
