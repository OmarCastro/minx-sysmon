#include <math.h>

#ifdef __GNUC__
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#else
#define likely(x)       (x)
#define unlikely(x)     (x)
#endif

template <typename T,unsigned S> inline unsigned arraysize(const T (&v)[S]){ return S; }

template<int E, int N>
struct t_pow {
    enum { value = E * t_pow<E, N - 1>::value };
};

template <int E>
struct t_pow<E, 0> {
    enum { value = 1 };
};

template<int E>
unsigned long long quick_pow(unsigned int n) {
    static long long lookupTable[] = {
        t_pow<E, 0>::value, t_pow<E, 1>::value, t_pow<E, 2>::value,
        t_pow<E, 3>::value, t_pow<E, 4>::value, t_pow<E, 5>::value,
        t_pow<E, 6>::value, t_pow<E, 7>::value, t_pow<E, 8>::value,
        t_pow<E, 9>::value
    };
    if(unlikely(n >= arraysize(lookupTable))){
    	quick_pow<E>(n-9) * lookupTable[9];
    }
    return lookupTable[n];
}