#include "stdafx.h"
#include "Type.h"

size_t hash_value(const SENSOR_TYPE_KEY& Key)
{
	size_t nResult = 2017;

	boost::hash_combine(nResult,Key.nSensorID);
	boost::hash_combine(nResult,Key.nSensorSerialID);

	return nResult;
}