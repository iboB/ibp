add_executable(scratch-ibp scratch.cpp)

target_link_libraries(scratch-ibp
    ibp::profile
    ibp::cvt-trace
)
