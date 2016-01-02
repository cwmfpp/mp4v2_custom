#ifndef _MEM_POOL_H_
#define _MEM_POOL_H_

#include "libdatacenter.h"

TDataPackage *allocator_alloc(int  _size);

void allocator_free(TDataPackage *_pPacket);



#endif

