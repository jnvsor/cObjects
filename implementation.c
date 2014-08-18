#include "cobjects.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
  int a;
} firstClass;

CONSTR(firstClass){
  return this;
}

DESTR(firstClass){
  (void) this; // Suppress unused variable warning
}


typedef struct {
  INHERIT(firstClass);
  char * string;
} textClass;

CONSTR(textClass, const char * string){
  // Call parent constructor on this struct
  P_CONSTR(firstClass);

  // Note that new memory has been allocated by strdup()
  this->string = strdup(string);

  // Access inherited class members by explicit cast
  CAST(this,firstClass)->a = 3;

  // And finally return
  return this;
}

DESTR(textClass){
  // Remember to delete things you allocated in the constructor
  free(this->string);

  /* Finally, always call destructor on parent class, even if it didn't allocate
   * anything. It may have parent classes that did */
  P_DESTR(firstClass);
}


typedef struct {
  INHERIT(textClass);
  int b;
} lastClass;

CONSTR(lastClass){
  // Calling parent constructor with arguments
  P_CONSTR(textClass, "Take a look at this string");

  printf("this.string is currently: %s\n", CAST(this,textClass)->string);

  /* Lets imagine we want to override the string at this point. Free the old one
   * allocate some new space and point to it, then copy in a new string. Note
   * that if you're using some kind of object to represent your string you
   * should delete it via whatever method it expects (For example if this string
   * were a cObject: DEL(CAST(this,initClass)->string) */
  free(CAST(this,textClass)->string);
  CAST(this,textClass)->string = malloc(20);
  strcpy(CAST(this,textClass)->string,"Whoop dee doo");

  return this;
}

DESTR(lastClass){
  /* Notice how we don't free the string we allocated in the constructor! We've
   * already done our part by freeing the string that was there in the first
   * place. The parent destructor will deallocate "Whoop dee doo" */
  P_DESTR(textClass);
}


int main(){
  // Simple data objects are just structs
  firstClass * F = NEW(firstClass);
  F->a = 12;
  printf("F.a == %d\n",F->a);
  DEL(F,firstClass);

  // Derived object with constructor
  lastClass * special = NEW(lastClass);

  printf(
    "special.a defaulted to: %d\nspecial.string defaulted to: %s\nspecial.b was not set: %d\n",
    CAST(special,firstClass)->a,
    CAST(special,textClass)->string,
    special->b
  );

  char * s = CAST(special,textClass)->string;
  int * a = &CAST(special,firstClass)->a;
  printf(
    "s now points to: %p which is: %s\na now points to %p which is %d\n",
    s, s, a, *a);

  DEL(special,lastClass);

  return 0;
}
