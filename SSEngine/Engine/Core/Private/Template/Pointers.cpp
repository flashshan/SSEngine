#include "Core\Template\Pointers.h"

template<typename From, typename To>
StrongPtr<To> CastStrongPtr(StrongPtr<From>& i_from)
{
	if (i_from)
	{
		return StrongPtr<To>(reinterpret_cast<To*>(&(*i_from)), i_from.GetCounter());
	}
	else
	{
		return StrongPtr<To>(nullptr);
	}
}

template<typename From, typename To>
WeakPtr<To> CastWeakPtr(WeakPtr<From>& i_from)
{
	if (i_from)
	{
		return WeakPtr<To>(CastStrongPtr<From, To>(i_from.Acquire()));
	}
	else
	{
		return WeakPtr<To>();
	}
}
