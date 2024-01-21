cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})
project(join_server VERSION ${PROJECT_VERSION})
configure_file(include/version.h.in version.h)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
include_directories(${GTEST_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR})

add_executable(join_server
    src/main.cpp
    src/parser.cpp
    src/table.cpp
    src/database.cpp)

add_executable(unit_tests 
    tests/tests.cpp
    src/table.cpp
)

set_target_properties(join_server PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

set_target_properties(unit_tests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

target_link_libraries(join_server
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_LIBRARIES})

target_link_libraries(unit_tests
        GTest::gtest_main
        pthread)

if (MSVC)
    target_compile_options(join_server PRIVATE
        /W4
    )
    target_compile_options(unit_tests PRIVATE
        /W4
    )
else ()
    target_compile_options(join_server PRIVATE
        -Wall -Wextra -pedantic -Werror 
    )
    target_compile_options(unit_tests PRIVATE
        -Wall -Wextra -pedantic -Werror 
    )
endif()

install(TARGETS join_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT agar10@yandex.ru)
include(CPack)

enable_testing()
add_test(unit_tests unit_tests)
include(GoogleTest)
gtest_discover_tests(unit_tests)


