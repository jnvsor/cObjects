#include "cobjects.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Ignore the lack of opening curly brace. It's a limitation of the
 * C macro system */

// firstClass
CLASS()
  int a;
} firstClass;

CONSTRUCTOR(firstClass)
  return this;
}

DESTRUCTOR(firstClass)
}


// initClass
SUBCLASS(firstClass)
  char * b;
} initClass;

CONSTRUCTOR(initClass, const char * string)
  P_NEW(firstClass);
  this->b = strdup(string); // Note that new memory has been allocated!
  CAST(this,firstClass)->a = 3;
  return this;
}

DESTRUCTOR(initClass)
  free(this->b);
  P_DELETE(firstClass);
}


// lastClass
SUBCLASS(initClass)
  int c;
} lastClass;

CONSTRUCTOR(lastClass)
  P_NEW(initClass, "Take a look at this string");
  return this;
}

DESTRUCTOR(lastClass)
  P_DELETE(initClass);
}


// Demonstration
int main(){
  // Simple data objects are just structs
  firstClass * F = NEW(firstClass);
  F->a = 12;
  printf("F.a == %d\n",F->a);
  DELETE(F,firstClass);
  
  // Derived object with constructor
  lastClass * special = NEW(lastClass);
  
  printf( "special.a defaulted to: %d\nspecial.b defaulted to: %s\nspecial.c was not set: %d\n",
          CAST(special,firstClass)->a,
          CAST(special,initClass)->b,
          special->c
        );
        
  char * s = CAST(special,initClass)->b;
  int * a = &CAST(special,firstClass)->a;
  printf("s now points to: %p which is: %s\na now points to %p which is %d\n",s,s,a,*a);
  
  DELETE(special,lastClass);
  
  printf("s still points to: %p which is now: %s\na still points to %p which is still: %d\n",s,s,a,*a);
  
  printf("a still points to %p which is now: %d\n",a,*a);
  
  return 0;
}
