#pragma once

#include <unordered_map>

#include "BasicTypes.h"

class ErrorMessage {
public:
	enum ERROR_TYPE
	{
		kNullPointer = 1001,
		kHeapOverflow = 1101,
		kStackOverflow = 1102,

		kNonPostiveValue = 2001,
		kNonZeroValue = 2002,
	};

	static FORCEINLINE ErrorMessage *GetInstance();
	FORCEINLINE const char *GetErrorMessage(const int32 i_errorCode) const;

private:
	FORCEINLINE ErrorMessage();
	void initialMessage();

	//static ErrorMessage *globalInstance;
private:
	std::unordered_map<int32, char *> messages_;
};




// implement forceinline

FORCEINLINE ErrorMessage::ErrorMessage()
{
	initialMessage();
}

FORCEINLINE ErrorMessage* ErrorMessage::GetInstance()
{
	static ErrorMessage *globalInstance;
	if (globalInstance == nullptr) {
		globalInstance = new ErrorMessage();
	}
	return globalInstance;
}

FORCEINLINE const char* ErrorMessage::GetErrorMessage(const int32 i_errorCode) const
{
	auto it = messages_.find(i_errorCode);

	if (it == messages_.end())
	{
		return "Wrong ErrorCode";
	}
	else
	{
		return it->second;
	}
}

