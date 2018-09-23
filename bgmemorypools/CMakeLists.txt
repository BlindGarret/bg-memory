set(
    SOURCES 
        "src/bulletpool.cxx"
        "include/bgmemorypools/bulletpool.hxx"
)

add_library(bgmemorypools ${SOURCES})
target_include_directories(bgmemorypools PUBLIC "include")
target_compile_features(bgmemorypools PUBLIC "cxx_std_14")

install(
    TARGETS bgmemorypools 
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include
)

add_subdirectory("test")