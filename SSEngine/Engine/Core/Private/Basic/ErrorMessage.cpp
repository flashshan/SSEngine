#include "Core\Basic\ErrorMessage.h"


void ErrorMessage::initialMessage()
{
	messages_.insert(std::pair<int32, char*>(kNullPointer, "Pointer point to NULL"));
	
	messages_.insert(std::pair<int32, char*>(kHeapOverflow, "Heap Over Flow"));
	messages_.insert(std::pair<int32, char*>(kStackOverflow, "Stack Over Flow"));


	messages_.insert(std::pair<int32, char*>(kNonPostiveValue, "Parameter Value below zero"));
	messages_.insert(std::pair<int32, char*>(kNonZeroValue, "Parameter value is not zero"));

}
