#include <stdlib.h>

/* Derived classes are implemented with parent class being a non-pointer struct
 * of the parent type, this lets us use parent members of undefined depth with a
 * simple pointer cast. Class declarations are ugly with a missing '{' but it's
 * still better than writing it all out by hand */ 
#define CLASS() typedef struct {
#define SUBCLASS(T) typedef struct { T parent;
#define CAST(A,B) ((B *)A)
#define OALLOC(T) malloc(sizeof(T))
#define OFREE(V,T) do{ \
                    T##_destroy(V); \
                    free(V); \
                    }while(0)
