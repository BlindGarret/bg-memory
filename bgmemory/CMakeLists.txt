set(
    SOURCES 
        "include/bgmemory/pointers/MutableUniquePtr.hxx"
        "include/bgmemory/DefaultDeleter.hxx"

        # "src/memoryfunctions.cxx"
        "src/bulletpool.cxx"

        # "include/bgmemory/bulletpool.hxx"
        # "include/bgmemory/assert.hxx"
        # "include/bgmemory/memoryfunctions.hxx"
)

add_library(bgmemory ${SOURCES})
target_include_directories(bgmemory PUBLIC "include")
target_compile_features(bgmemory PUBLIC "cxx_std_14")

install(
    TARGETS bgmemory 
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include
)

add_subdirectory("test")