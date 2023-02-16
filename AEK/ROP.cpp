#include "ROP.h"

namespace aek{
	std::optional<ROPChain> ROP::GetROPChain() {
		ROPChain rop_chain;
		auto chain_size = chain.size();
		if (chain_size == 0) {
			return std::nullopt;
		}

		if (new_rop != nullptr) {
			new_rop.reset();
		}
		new_rop = std::make_unique<UINT64[]>(chain_size);

		for (size_t i = 0; i < chain_size; i++) {
			new_rop[i] = chain[i];
		}
		rop_chain.rop = new_rop.get();
		rop_chain.size = chain_size;
		return rop_chain;
	}

	ROP& ROP::operator<<(const UINT64 gadget) {
		chain.push_back(gadget);
		return *this;
	}
	ROP& ROP::operator<<(const PVOID gadget) {
		chain.push_back((UINT64)gadget);
		return *this;
	}

	ROP& ROP::operator>>(const UINT64 values) {
		chain.push_back(values);
		return *this;
	}
	ROP& ROP::operator>>(const PVOID values) {
		chain.push_back((UINT64)values);
		return *this;
	}

	UINT64* ROP::operator()() {
		auto rop_chain = GetROPChain();
		if (rop_chain == std::nullopt)
			return nullptr;

		return rop_chain.value().rop;
	}

	size_t ROP::GetSize() {
		auto rop_chain = GetROPChain();
		if (rop_chain == std::nullopt)
			return 0;

		return rop_chain.value().size;
	}

	BOOL ROP::reset() {
		chain.clear();
		new_rop.reset();
	}
}