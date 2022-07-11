#ifndef __RPN_H__
#define __RPN_H__

#define STACK_LEN 4

typedef struct _val {
   int s;
   unsigned int i;
   unsigned int f;
   unsigned int p;
   int e;
   int flags;
} fp_comp_t;
extern struct _val zero_fp;

fp_comp_t key_process(fp_comp_t d, char in);

#endif
