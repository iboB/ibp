CPMAddPackage(gh:Cyan4973/xxHash@0.8.1)
add_library(ibp-xxhash INTERFACE)
add_library(ibp::xxhash ALIAS ibp-xxhash)
target_include_directories(ibp-xxhash INTERFACE ${xxHash_SOURCE_DIR})
