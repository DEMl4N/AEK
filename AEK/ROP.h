#pragma once
#include <Windows.h>
#include <vector>
#include <optional>
#include <memory>

namespace aek {
	typedef struct ROPChain {
		UINT64* rop;
		size_t size;
	} ROPChain;

	class ROP {
	private:
		//variables
		std::vector<UINT64> chain;
		UINT64 krnl_base = 0;
		std::unique_ptr<UINT64[]> new_rop;

		// method
		std::optional<ROPChain> GetROPChain();
	public:
		ROP(UINT64 krnl_base) : krnl_base(krnl_base) {
			chain.reserve(4);
		};
		ROP(UINT64 krnl_base, size_t gadget_count) : krnl_base(krnl_base) {
			chain.reserve(gadget_count);
		};

		ROP& operator<<(const UINT64 gadget);	// chain gadgets
		ROP& operator<<(const PVOID gadget);	// chain gadgets
		ROP& operator>>(const UINT64 value);	// chain values
		ROP& operator>>(const PVOID value);		// chain values
		UINT64* operator()();					// return rop data pointer
		size_t GetSize();
		BOOL reset();

	};

}