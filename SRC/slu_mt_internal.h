#ifndef SLU_MT_INTERNAL_H
#define SLU_MT_INTERNAL_H

/* -------------------------------------------------------------------------- */
/* pragmas */
/* -------------------------------------------------------------------------- */
#if defined(SUPPORT_IVDEP)
	#define SLU_MT_PRAGMA_IVDEP _Pragma( "ivdep" )
#elif defined(SUPPORT_LOOP_IVDEP)
	#define SLU_MT_PRAGMA_IVDEP _Pragma( "loop( ivdep )" )
#elif defined(SUPPORT_GCC_IVDEP)
	#define SLU_MT_PRAGMA_IVDEP _Pragma( "GCC ivdep" )
#elif defined(SUPPORT_LOOP_VECTORIZE)
	#define SLU_MT_PRAGMA_IVDEP _Pragma( "clang loop vectorize(enable)" )
#else
	#define SLU_MT_PRAGMA_IVDEP
#endif

#endif