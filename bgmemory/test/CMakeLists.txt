set(
    TESTS 
        "src/pointers/TestHelpers.cxx"
        "src/pointers/MutableSharedPtr.cxx"
        "src/pointers/MutableWeakPtr.cxx"
        "src/pointers/SharedPtrMutator.cxx"
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory($ENV{GOOGLE_TEST_SRC_DIR} tests)
add_executable(tests ${TESTS})
target_link_libraries(tests bgmemory gtest gtest_main gmock pthread)