// Copyright [2019] BlindGarret<lroe2930@gmail.com>
#ifndef BGMEMORY_INCLUDE_BGMEMORY_POINTERS_MUTABLEUNIQUEPTR_HXX_
#define BGMEMORY_INCLUDE_BGMEMORY_POINTERS_MUTABLEUNIQUEPTR_HXX_

#include <memory>
#include "bgmemory/DefaultDeleter.hxx"

#ifdef BG_MEMORY_MULTITHREAD

#endif  // BG_MEMORY_MULTITHREAD

namespace bg {

/*
    Unique pointer class, to be used as a drop in replacement for the
    standard library unique_ptr. The main difference is the ability
    to mutate the underlying pointer for it and all of its weak child
    pointers.

    The reason for this class is that some of our memory pool structures
    require the ability to swap out a pointer, in order to defragment a
    heap or some such functionality, and have that swap effect not just
    the current reference, but also effect the child references which are
    pointing at the current referece. Swap and reset do not accomplish
    this as they only mutate the current reference.

    This should not be used as a replacement for smart pointers without
    good cause. It will be used throughout this library for consistency,
    but the standard implementation is far better tested, and likely
    simply better. Use this implementation only if you have a good reason.

    Deleter is automatically setup to use the standard delete() function,
    but can be replaced with a function object in place of the default deleter.

    When BG_MEMORY_MULTITHREAD is defined the pointer will start using mutex
    locks around assignments and deletion. As with all smart pointers, there is
    no guarantee of thread safety with the stored object in memory. Any
   reference counting and deletion will be thread safe. The object will only be
   deleted once, and references will be atomically decremented and incremented.
   No other guarantees are made.
*/
template <class T, class DeleterT = DefaultDeleter<T>>
class MutableUniquePtr {
  struct PointerPayload {
    T *managedObject = nullptr;
    DeleterT deleter;
    bool isActive = false;
  };

  PointerPayload *payload = nullptr;

 public:
  /*
      Constructs a unique pointer with no owned object.

      This constructor will use the deleter object using the default
      constructor, so it requires a default constructable deleter with a
      noexcept guarantee.
  */
  MutableUniquePtr() {
    payload = new PointerPayload();
    payload->deleter = DeleterT();
  }

  /*
      Constructs a unique pointer with no owned object.

      This constructor will use the deleter object using the default
      constructor, so it requires a default constructable deleter with a
      noexcept guarantee.

  */
  constexpr MutableUniquePtr(std::nullptr_t) noexcept {  // NOLINT
    // todo: implement
  }

  /*
      Constructs a unique pointer which takes ownership of the pointer
      passed to it.

      This constructor will use the deleter object using the default
      constructor, so it requires a default constructable deleter with a
      noexcept guarantee.

      @param pointer the pointer to take ownership of.
  */
  explicit MutableUniquePtr(T *pointer) noexcept {
    // todo: implement
  }

  /*
      Constructs a unique pointer which takes ownership of the pointer
      passed to it.

      This constructor will copy construct your deleter reference passed
      into the function. The expected copy constructor must be marked
      noexcept.

      @param pointer the pointer to take ownership of.
      @param d a reference to an instance of the deleter.
  */
  MutableUniquePtr(T *pointer, const DeleterT &d) noexcept {
    // todo: implement
  }

  /*
      Constructs a unique pointer which takes ownership of the pointer
      passed to it.

      Thir constructor will move construct your deleter reference passed
      into the function. The expected move constructor must be marked
      noexcept.


      @param pointer the pointer to take ownership of.
      @param d an r-value reference to an instance of the deleter.
  */
  MutableUniquePtr(T *pointer, DeleterT &&d) noexcept {
    // todo: implement
  }

  /*
      Disallow copying.
  */
  MutableUniquePtr(MutableUniquePtr<T, DeleterT> &) = delete;

  ~MutableUniquePtr() {
    payload->deleter(payload->managedObject);
  }

  /*
      Releases ownership of the managed memory, if any, without triggering
      cleanup.

      @return the pointer to the contained object.
  */
  T *release() noexcept {
    // todo: implement
  }

  /*
      Replaces the managed object, cleaning up the previous pointer.

      @param ptr pointer to the object to take ownership of.
  */
  void reset(T *ptr) noexcept {
    // todo: implement
  }

  /*
      Cleans up the current managed object, then replaces it with the
      new pointer. Unlike reset this changes the underlying pointer in
      all derived weak pointers as well.

      @param ptr the pointer to take ownership of.
  */
  void mutate(T *ptr) noexcept {
    // todo: implement
  }

  /*
      Swaps ownership of objects between this instance and the
      given one.

      @param other the instace of the unique pointer to swap with.
  */
  void swap(MutableUniquePtr<T, DeleterT>  // NOLINT
                &other) noexcept {
    // todo: implement
  }

  /*
      Gets a pointer to the managed object.

      @return raw pointer to the managed object.
  */
  T *get() const noexcept {
    // todo: implement
  }

  /*
      Gets a reference to the deleter defined for this instance.

      @return reference to the defined deleter.
  */
  DeleterT &getDeleter() noexcept {
    // todo: implement
  }

  /*
      Gets a constant reference to the deleter defined for this
      instance.

      @return constant reference to the defined deleter.
  */
  const DeleterT &getDeleter() const noexcept {
    // todo: implement
  }

  /*
      Checks whether there is an object being managed, or whether
      the object has been released or never set in the first place.

      @return whether there is an object being managed.
  */
  explicit operator bool() const noexcept {
    // todo: implement
  }

  /*
      Provides access to undlying reference controlled by smart
      pointer.

      If underlying memory is released or uninitialized the results
      of this function are undefined, like accessing uninitialized memory.

      @return l-value reference to the underlying memory.
  */
  typename std::add_lvalue_reference<T>::type operator*() const {
    // todo: implement
  }

  /*
      Provides access to undlying reference controlled by smart
      pointer.

      If underlying memory is released or uninitialized the results
      of this function are undefined, like accessing uninitialized memory.

      @return reference to the underlying memory.
  */
  T *operator->() const noexcept {
    // todo: implement
  }
};
}  // namespace bg

#endif  // BGMEMORY_INCLUDE_BGMEMORY_POINTERS_MUTABLEUNIQUEPTR_HXX_
