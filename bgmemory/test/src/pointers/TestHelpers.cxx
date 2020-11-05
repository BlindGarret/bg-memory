#include "./TestHelpers.hxx"

int TrackedDeletableTestObject::liveObjectCount;
int CountableTestDeleter::deleteCount;
int CountableTestDeleter::constructCount;