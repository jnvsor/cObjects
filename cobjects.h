#include <stdlib.h>

#define CLASS() typedef struct {
#define SUBCLASS(T) typedef struct { T parent;

#define CAST(A,B) ((B *)A)
#define OALLOC(T) malloc(sizeof(T))

#define CONSTRUCTOR(T,...) T * T##_new(__VA_ARGS__){ T * this = OALLOC(T);
#define DESTRUCTOR(T) void T##_destroy(T * this){

#define P_NEW(T,...) do{ T * p = T##_new(__VA_ARGS__); this->parent = *p; free(p) ;}while(0)
#define P_DELETE(T) T##_destroy((T *)this)

#define NEW(T,...) T##_new(__VA_ARGS__)
#define DELETE(V,T) do{ T##_destroy(V); free(V); }while(0)
