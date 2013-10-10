#include "cobjects.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CLASS()
  int a;
} firstClass;


SUBCLASS(firstClass)
  char * b;
} initClass;

initClass * initClass_new(const char * string){
  initClass * self = OALLOC(initClass);
  
  self->b = strdup(string); // Note that new memory has been allocated!
  
  
  /* As the parent class is the first element in the struct the pointer is valid
   * for ALL parents of this class all the way up the hierarchy, you just have
   * to cast it. This has the limitation that multiple inheritance isn't
   * possible with this system */
  CAST(self,firstClass)->a = 3;
  return self;
}

void initClass_destroy(initClass * self){
  /* Destructors should NOT free themselves, this could free child objects.
   * Instead, simply call the parent destructor and free any items in self that
   * were malloc()ed into existance in the constructor. In this case the
   * constructor's strdup() call malloc()ed a string b */
  free(self->b);
}

SUBCLASS(initClass)
  int c;
} notInitClass;

notInitClass * notInitClass_new(){
  notInitClass * self = OALLOC(initClass);
  
  /* All derived classes should call parent constructor if at all possible, if
   * parent has no constructor, you'd better hope niether it nor it's parents
   * need one. Ever. For example, while this class has no self-made purpose for
   * a constructor (Other than presenting initClass with a string), its parent
   * initClass requires initialization, so this one needs a constructor too */
  
  initClass * p = initClass_new("Take a look at this string");
  
  /* Copy class to parent */
  self->parent = *p;
  
  /* While any class with a constructor should have a destructor, if only for
   * safety, we should free() the parent class rather than destruct it as any
   * copied pointers should be retained and will be destroyed when this object
   is */
  free(p);
  
  return self;
}

void notInitClass_destroy(notInitClass * self){
  /* Destructors should NOT free themselves, this could free child objects.
   * Instead, simply call the parent destructor and free any items in self that
   * were malloc()ed into existance in the constructor. */
  initClass_destroy(CAST(self,initClass));
  
  // No members were malloc()ed so just return
}


int main(){
  // Simple data objects are just structs
  firstClass * F = OALLOC(firstClass);
  F->a = 12;
  printf("F.a == %d\n",F->a);
  free(F);
  /* This object has no destructor. If we want to use the more generic OFREE()
   * destruction method we'll need to make a type_destroy() function for ALL
   * classes, including the ones that don't need one like this */
  
  // Derived object with constructor
  notInitClass * special = notInitClass_new();
  
  printf( "special.a defaulted to: %d\nspecial.b defaulted to: %s\nspecial.c was not set: %d\n",
          CAST(special,firstClass)->a,
          CAST(special,initClass)->b,
          special->c
        );
        
  char * s = CAST(special,initClass)->b;
  int * a = &CAST(special,firstClass)->a;
  printf("s now points to: %p which is: %s\na now points to %p which is %d\n",s,s,a,*a);
  
  notInitClass_destroy(special);
  
  printf("s still points to: %p which is now: %s\na still points to %p which is still: %d\n",s,s,a,*a);
  
  // Note how the destructor hasn't free()d the object it'self, that's up to us:
  
  free(special);
  
  printf("a still points to %p which is now: %d\n",a,*a);
  
  /* Note the OFREE() macro that does the destroy and the free. This is the only
   * macro in the system that places a limitation on naming schemes (IE: it
   * requires the function declaration be type_destroy(type) ) but this is
   * a purely cosmetic change and if you'd rather specify the whole function
   * name feel free to change the macro */
  special = notInitClass_new();
  OFREE(special,notInitClass);
  
  return 0;
}
