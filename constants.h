
#ifndef CONSTANTS_H_INCLUDED_
#define CONSTANTS_H_INCLUDED_

#include <stddef.h>

typedef ptrdiff_t tag_t; //64 bit!
#define WORD_SIZE 8 //(sizeof(Operand))
#define NAKED_WORD_SIZE 4 //(sizeof(Operand) - sizeof(OPTYPE))

#define OBJ_HEADER_SIZE ((int)(sizeof(void*) + sizeof(tag_t) * 2))
#define OBJDATA_HEADER_SIZE ((int)sizeof(int))
#define ARRAYOBJ_HEADER_SIZE ((int)(sizeof(void*) * 2 + sizeof(int) * 2))
#define STRINGOBJ_HEADER_SIZE ((int)(sizeof(void*) * 2 + sizeof(int)))
#define STACK_LOCALS_OFFSET 0
#define STACK_OLD_EBP_OFFSET -1
#define STACK_RA_OFFSET -2
#define STACK_THIS_OFFSET -3
#define STACK_PARAMS_OFFSET -4

#define CALL_PARAMS_OFFSET -2
#define BAD_PTR 0xbaadf00d

#endif //CONSTANTS_H_INCLUDED_

