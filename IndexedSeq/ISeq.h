#ifndef ISEQ_H_INCLUDED
#define ISEQ_H_INCLUDED

#include <stdlib.h>

typedef void* any_t;

struct _ISeq {
  size_t  sz;
  size_t  used;
  any_t*  ind;
};
typedef struct _ISeq ISeq;

#define ISeq_INIT_SZ  1024


ISeq* ISeq_new(void);
void ISeq_free(ISeq** iseq);
void ISeq_deepfree(ISeq** iseq);
ISeq* ISeq_app(ISeq* iseq, any_t elem);
ISeq* ISeq_put(ISeq* iseq, size_t ind, any_t elem);
ISeq* ISeq_rm(ISeq* iseq, size_t ind);
ISeq* ISeq_deepcopy(ISeq* iseq);



#endif // ISEQ_H_INCLUDED
