#include <stm32f10x.h>
#include "sysctl.h"

#if defined(ewarm) || defined(DOXYGEN)
void SysCtlDelay(uint32_t ui32Count)
{
	__asm("    subs    r0, #1\n"
	"    bne.n   SysCtlDelay\n"
		"    bx      lr");
}
#endif
#if defined(codered) || defined(gcc) || defined(sourcerygxx)
void __attribute__((naked)) SysCtlDelay(uint32_t ui32Count)
{
	__asm("    subs    r0, #1\n"
	"    bne     SysCtlDelay\n"
		"    bx      lr");
}
#endif
#if defined(rvmdk) || defined(__ARMCC_VERSION)
__asm void SysCtlDelay(uint32_t ui32Count)
{
	subs    r0, #1;
	bne     SysCtlDelay;
	bx      lr;
}
#endif
