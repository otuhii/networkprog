#include <stdio.h>
#include <stdlib.h>
#include "garbage.h"
#include <assert.h>
//simple mark&sweep gc, maybe in future i will try something more sophisticated(maybe tri-color)
//disadvs - during sweeping or scanning in general(with marking stage)(im not sure) whole program 
//gets freezed so it can be bad for online or for time-critical apps  


//https://journal.stuffwithstuff.com/2013/12/08/babys-first-garbage-collector/ - followed this article


VM* newVM()
{
  VM* vm = malloc(sizeof(VM));
  vm->stackSize = 0;
  vm->firstObject = NULL;
  vm->objectCount = 0;
  vm->maxObjects = GC_THRESHOLD;
  return vm;
}

void push(VM* vm, Object* value)
{
  assert(vm->stackSize < STACK_MAX);
  vm->stack[vm->stackSize++] = value;
}

Object* pop(VM* vm)
{
  assert(vm->stackSize > 0);
  return vm->stack[--vm->stackSize];
}


Object* newObject(VM* vm, ObjectType type)
{
  if (vm->objectCount >= vm->maxObjects)
    gc(vm);

  Object* newObj = malloc(sizeof(Object));
  newObj->type = type;
  newObj->marked = 0;
  vm->objectCount++;

  newObj->next = vm->firstObject;
  vm->firstObject = newObj;

  return newObj;
}


void pushInt(VM* vm, int intValue)
{
  Object* obj = newObject(vm, OBJ_INT);
  obj->value = intValue;
  push(vm, obj);
}

void pushPair(VM* vm)
{
  Object* obj = newObject(vm, OBJ_PAIR);
  obj->tail = pop(vm);
  obj->head = pop(vm);
  push(vm, obj);
}


void mark(Object* obj)
{
  if(!obj || obj->marked) return;

  obj->marked = 1;

  if (obj->type == OBJ_PAIR){
    mark(obj->head);//recursion yay!!!!
    mark(obj->tail);
  }
}

void markAll(VM* vm)
{
  for (int i = 0; i < vm->stackSize;i++)
    mark(vm->stack[i]);
}


void sweep(VM* vm)
{
  Object** object = &vm->firstObject;
  while (*object)
  {
    if (!(*object)->marked)
    {
      Object* toSweep = *object;

      *object = toSweep->next;
      free(toSweep);
      
      vm->objectCount--;
    }
    else
    {
      (*object)->marked = 0;
      object = &(*object)->next;
    }
  }
}

void gc(VM* vm)
{
  markAll(vm);
  sweep(vm);

  vm->maxObjects = vm->objectCount * 2;

}


void freeVM(VM* vm)
{
  vm->stackSize = 0;
  gc(vm);
  free(vm);
}

void objectPrint(Object* object) {
  switch (object->type) {
    case OBJ_INT:
      printf("%d", object->value);
      break;

    case OBJ_PAIR:
      printf("(");
      objectPrint(object->head);
      printf(", ");
      objectPrint(object->tail);
      printf(")");
      break;
  }
}


