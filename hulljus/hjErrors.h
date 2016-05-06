#pragma once

#include "hjApp.h"

namespace hjErrors {

	enum ErrorCode {
		CannotConvertConnodeToType = 0,
		CannotConvertVarnodeToType = 1,
		NotEnoughConnodes = 2,
		NotEnoughParameters = 3
	};

	void reportError(gennode*, ErrorCode);

}