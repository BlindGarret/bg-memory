set(
    TESTS 
        # "src/example.cxx"
        # "src/memoryfunctions.cxx"
        "src/main.cxx"
        "src/pointers/MutableUniquePtr.cxx"
)

add_executable(tests ${TESTS})

set_target_properties( tests
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
)


target_link_libraries(tests bgmemory gtest gmock)