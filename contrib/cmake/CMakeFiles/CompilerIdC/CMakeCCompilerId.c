#ifdef __cplusplus
# error "A C++ compiler has been selected for C."
#endif

#if defined(__18CXX)
# define ID_VOID_MAIN
#endif

#if defined(__INTEL_COMPILER) || defined(__ICC)
# define COMPILER_ID "Intel"

#elif defined(__BORLANDC__)
# define COMPILER_ID "Borland"

#elif defined(__WATCOMC__)
# define COMPILER_ID "Watcom"

#elif defined(__SUNPRO_C)
# define COMPILER_ID "SunPro"

#elif defined(__HP_cc)
# define COMPILER_ID "HP"

#elif defined(__DECC)
# define COMPILER_ID "Compaq"

#elif defined(__IBMC__)
# define COMPILER_ID "VisualAge"

#elif defined(__PGI)
# define COMPILER_ID "PGI"

#elif defined(__GNUC__)
# define COMPILER_ID "GNU"

#elif defined(_MSC_VER)
# define COMPILER_ID "MSVC"

#elif defined(__ADSPBLACKFIN__) || defined(__ADSPTS__) || defined(__ADSP21000__)
/* Analog Devices C++ compiler for Blackfin, TigerSHARC and
   SHARC (21000) DSPs */
# define COMPILER_ID "ADSP"

/* IAR Systems compiler for embedded systems.
   http://www.iar.com
   Not supported yet by CMake
#elif defined(__IAR_SYSTEMS_ICC__)
# define COMPILER_ID "IAR" */

/* sdcc, the small devices C compiler for embedded systems,
   http://sdcc.sourceforge.net  */
#elif defined(SDCC)
# define COMPILER_ID "SDCC"

#elif defined(_COMPILER_VERSION)
# define COMPILER_ID "MIPSpro"

/* This compiler is either not known or is too old to define an
   identification macro.  Try to identify the platform and guess that
   it is the native compiler.  */
#elif defined(__sgi)
# define COMPILER_ID "MIPSpro"

#elif defined(__hpux) || defined(__hpua)
# define COMPILER_ID "HP"

#else /* unknown compiler */
# define COMPILER_ID ""

#endif

/* Construct the string literal in pieces to prevent the source from
   getting matched.  Store it in a pointer rather than an array
   because some compilers will just produce instructions to fill the
   array rather than assigning a pointer to a static array.  */
char* info_compiler = "INFO" ":" "compiler[" COMPILER_ID "]";

/* Identify known platforms by name.  */
#if defined(__linux) || defined(__linux__) || defined(linux)
# define PLATFORM_ID "Linux"

#elif defined(__CYGWIN__)
# define PLATFORM_ID "Cygwin"

#elif defined(__MINGW32__)
# define PLATFORM_ID "MinGW"

#elif defined(__APPLE__)
# define PLATFORM_ID "Darwin"

#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
# define PLATFORM_ID "Windows"

#elif defined(__FreeBSD__) || defined(__FreeBSD)
# define PLATFORM_ID "FreeBSD"

#elif defined(__NetBSD__) || defined(__NetBSD)
# define PLATFORM_ID "NetBSD"

#elif defined(__OpenBSD__) || defined(__OPENBSD)
# define PLATFORM_ID "OpenBSD"

#elif defined(__sun) || defined(sun)
# define PLATFORM_ID "SunOS"

#elif defined(_AIX) || defined(__AIX) || defined(__AIX__) || defined(__aix) || defined(__aix__)
# define PLATFORM_ID "AIX"

#elif defined(__sgi) || defined(__sgi__) || defined(_SGI)
# define PLATFORM_ID "IRIX"

#elif defined(__hpux) || defined(__hpux__)
# define PLATFORM_ID "HP-UX"

#elif defined(__HAIKU) || defined(__HAIKU__) || defined(_HAIKU)
# define PLATFORM_ID "Haiku"
/* Haiku also defines __BEOS__ so we must 
   put it prior to the check for __BEOS__
*/

#elif defined(__BeOS) || defined(__BEOS__) || defined(_BEOS)
# define PLATFORM_ID "BeOS"

#elif defined(__QNX__) || defined(__QNXNTO__)
# define PLATFORM_ID "QNX"

#elif defined(__tru64) || defined(_tru64) || defined(__TRU64__)
# define PLATFORM_ID "Tru64"

#elif defined(__riscos) || defined(__riscos__)
# define PLATFORM_ID "RISCos"

#elif defined(__sinix) || defined(__sinix__) || defined(__SINIX__)
# define PLATFORM_ID "SINIX"

#elif defined(__UNIX_SV__)
# define PLATFORM_ID "UNIX_SV"

#elif defined(__bsdos__)
# define PLATFORM_ID "BSDOS"

#elif defined(_MPRAS) || defined(MPRAS)
# define PLATFORM_ID "MP-RAS"

#elif defined(__osf) || defined(__osf__)
# define PLATFORM_ID "OSF1"

#elif defined(_SCO_SV) || defined(SCO_SV) || defined(sco_sv)
# define PLATFORM_ID "SCO_SV"

#elif defined(__ultrix) || defined(__ultrix__) || defined(_ULTRIX)
# define PLATFORM_ID "ULTRIX"

#elif defined(__XENIX__) || defined(_XENIX) || defined(XENIX)
# define PLATFORM_ID "Xenix"

#else /* unknown platform */
# define PLATFORM_ID ""

#endif

/* Construct the string literal in pieces to prevent the source from
   getting matched.  Store it in a pointer rather than an array
   because some compilers will just produce instructions to fill the
   array rather than assigning a pointer to a static array.  */
char* info_platform = "INFO" ":" "platform[" PLATFORM_ID "]";


/*--------------------------------------------------------------------------*/

#ifdef ID_VOID_MAIN
void main() {}
#else
int main(int argc, const char* argv[])
{
  int require = 0;
  require += info_compiler[argc];
  require += info_platform[argc];
  (void)argv;
  return require;
}
#endif
