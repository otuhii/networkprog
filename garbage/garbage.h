#define STACK_MAX 256
#define GC_THRESHOLD 200



//creating types for vm
typedef enum {
  OBJ_INT,
  OBJ_PAIR
} ObjectType;

typedef struct sObject
{
  ObjectType type;
  
  struct sObject* next;

  unsigned char marked;

  union
  {
    int value;

    struct {
      struct sObject* head; 
      struct sObject* tail;
    };
  };

}Object;



//some really really naive vm, with just a stack
typedef struct {
  int objectCount;//objects in memory in general(also garbage, in stackSize only objects that are curently being used, its really easy to see but i had troubles seeing that)
  int maxObjects;//amount of objs needed to call gc

  Object* stack[STACK_MAX];
  int stackSize;

  Object* firstObject;
} VM;


VM* newVM();
void freeVM(VM* vm);
void push(VM* vm, Object* value);
Object* pop(VM* vm);
Object* newObject(VM* vm, ObjectType type);
void pushInt(VM* vm, int intValue);
void pushPair(VM* vm);
void mark(Object* obj);
void markAll(VM* vm);
void sweep(VM* vm);
void gc(VM* vm);
void objectPrint(Object* object);


