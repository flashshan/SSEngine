#include "Core\Basic\ErrorMessage.h"


void ErrorMessage::initialMessage()
{
	const int32 maxRecords = 1000;
	messages_.reserve(maxRecords);

	messages_.insert(std::pair<int32, char*>(static_cast<int32>(ErrorType::ENullPointer), "Pointer point to NULL"));
	
	messages_.insert(std::pair<int32, char*>(static_cast<int32>(ErrorType::EHeapOverflow), "Heap Over Flow"));
	messages_.insert(std::pair<int32, char*>(static_cast<int32>(ErrorType::EStackOverflow), "Stack Over Flow"));

	messages_.insert(std::pair<int32, char*>(static_cast<int32>(ErrorType::ENonPostiveValue), "Parameter Value below zero"));
	messages_.insert(std::pair<int32, char*>(static_cast<int32>(ErrorType::ENonZeroValue), "Parameter value is not zero"));

}
