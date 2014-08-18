#ifndef __cObjects_H
#define __cObjects_H

#include <stdlib.h>

/* Used to define base class in struct. Must be first element of struct. */
#define INHERIT(T) T __cObjects_parent

/* For easy casting between objects */
#define CAST(A,B) ((B *)A)

/* Constructor, destructor */
#define CONSTR(T,...) T * T##_new(T * this, ##__VA_ARGS__)
#define DESTR(T) void T##_destroy(T * this)

/* NEW(T,...) and DELETE(P,T) for object creation, deletion */
#define NEW(T,...) T##_new(malloc(sizeof(T)), ##__VA_ARGS__)
#define DEL(P,T) do{ T##_destroy(P); free(P); }while(0)

/* P_CONSTR(T,...) and P_DESTR(T) for use in constructors to initialize the
 * parent object */
#define P_CONSTR(T,...) T##_new(CAST(this,T), ##__VA_ARGS__)
#define P_DESTR(T) T##_destroy(CAST(this,T))

#endif
