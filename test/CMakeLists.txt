
enable_testing()

set(testfiles   stringlength    )


foreach(t ${testfiles})
    add_executable(${t} ${t}.c include.h)
    target_link_libraries(${t} wcrt-static)
    add_dependencies(${t} wcrt-static)
    add_test(${t}-test  ${t})
endforeach()

