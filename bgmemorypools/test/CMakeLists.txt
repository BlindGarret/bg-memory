set(
    TESTS 
        "src/main.cxx"
        "src/example.cxx"
)
add_executable(tests ${TESTS})
target_link_libraries(tests bgmemorypools gtest gmock)