// Copyright [2019] BlindGarret<lroe2930@gmail.com>
#ifndef TEST_POINTERS_TESTHELPERS_HXX_
#define TEST_POINTERS_TESTHELPERS_HXX_

template <class T>
class IdTestDeleter
{
  static int nextId;
  int id;

public:
  IdTestDeleter()
  {
    id = IdTestDeleter::nextId;
    IdTestDeleter::nextId++;
  }

  static void reset()
  {
    nextId = 0;
  }

  int getId() const
  {
    return id;
  }
  
  void operator()(T *pointer) {
    if (pointer != nullptr) {
      delete pointer;
    }
  }
};

template <class T>
int IdTestDeleter<T>::nextId;

class CountableTestDeleter
{
  static int deleteCount;
  static int constructCount;

public:
  CountableTestDeleter()
  {
    CountableTestDeleter::constructCount++;
  }

  static int getDeleteCount() { return deleteCount; }
  static int getConstructCount() { return constructCount; }

  static void reset()
  {
    deleteCount = 0;
    constructCount = 0;
  }

  void operator()(int *pointer)
  {
    CountableTestDeleter::deleteCount++;
    if (pointer != nullptr)
    {
      delete pointer;
    }
  }
};

int CountableTestDeleter::deleteCount;
int CountableTestDeleter::constructCount;

class TrackedDeletableTestObject
{
  static int liveObjectCount;

public:
  TrackedDeletableTestObject()
  {
    TrackedDeletableTestObject::liveObjectCount++;
  }

  ~TrackedDeletableTestObject()
  {
    TrackedDeletableTestObject::liveObjectCount--;
  }

  static void reset()
  {
    liveObjectCount = 0;
  }

  static int getLiveObjectCount() { return liveObjectCount; }
};

int TrackedDeletableTestObject::liveObjectCount;

class SimpleTestObject
{
  int value;

public:
  SimpleTestObject(int v)
  {
    value = v;
  }

  int GetValue() const
  {
    return value;
  }
};

#endif // TEST_POINTERS_TESTHELPERS_HXX_