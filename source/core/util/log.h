#include "../../ext/format/format.h"

#if defined(RELEASE) || defined(NK_RELEASE)
    #define nkLog(args...) ;
    #define nkAssert(x,args...) ;
#else
#if NK_SUPPRESS_LOGS
       #define nkLog(args...) ;
    #else
#define nkLog(args...) fmt::printf(args); printf("\n");
#endif
#define nkAssert(x,args...) if (!x){printf("ASSERT!: "); nkLog(args); assert(0);}
#endif

#define nkError(args...) printf("nk_error: "); fmt::printf(args); printf("\n");
