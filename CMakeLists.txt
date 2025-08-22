# CMakeList.txt : CMake project for Intro2DGame_0, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Intro2DGame")

# Add source to this project's executable.
add_executable (Intro2DGame "main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Intro2DGame PROPERTY CXX_STANDARD 20)
endif()


target_link_libraries(Intro2DGame PRIVATE 
    sfml-main
)

target_include_directories(Intro2DGame PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/x64-windows/include
)
