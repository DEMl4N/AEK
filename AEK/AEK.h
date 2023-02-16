#pragma once
#include <Windows.h>
#include <iostream>
#include <winternl.h>
#include <Psapi.h>
#include "Leaker.h"
#include "Exploiter.h"
#include "IoManager.h"

namespace aek {
	static IO io;
	static Leaker lk;
	static Exploiter ex(lk);
}