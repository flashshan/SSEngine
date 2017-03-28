#pragma once

#include <unordered_map>

#include "BasicTypes.h"

enum class ErrorType : int32 {
	// Pointer issue
	ENullPointer = 1001,

	// Memory issue
	EHeapOverflow = 1101,
	EStackOverflow = 1102,

	// Value issue
	ENonPostiveValue = 2001,
	ENonZeroValue = 2002,
};

class ErrorMessage {
public:
	static inline ErrorMessage *GetInstance();
	FORCEINLINE const char *GetErrorMessage(const ErrorType i_errorCode) const;

private:
	inline ErrorMessage();
	void initialMessage();

	//static ErrorMessage *globalInstance;
private:
	std::unordered_map<int32, char *> messages_;
};




// implement forceinline

inline ErrorMessage::ErrorMessage()
{
	initialMessage();
}

inline ErrorMessage* ErrorMessage::GetInstance()
{
	static ErrorMessage *globalInstance;
	if (globalInstance == nullptr) {
		globalInstance = new ErrorMessage();
	}
	return globalInstance;
}

FORCEINLINE const char* ErrorMessage::GetErrorMessage(const ErrorType i_errorCode) const
{
	auto it = messages_.find(static_cast<int32>(i_errorCode));

	if (it == messages_.end())
	{
		return "Wrong ErrorCode";
	}
	else
	{
		return it->second;
	}
}

