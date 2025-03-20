# CMakeList.txt : CMake project for FloppyLib, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("FloppyLib" VERSION 0.0.1 DESCRIPTION "A library to process floppy disk images.")

SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Add source to this project's executable.
add_library (FloppyLib  SHARED
	"src/CylinderHeadSector.cpp"
	"src/Mode.cpp"
	"src/Sector.cpp"
	"src/Track.cpp"
	"src/Disk.cpp"
	"src/ImdFunctions.cpp"
	"src/InterchangeFile.cpp"
	"src/InterchangeVolume.cpp"
	"src/FloppyLib.cpp"
	"src/DiskStatistics.cpp"
	"src/InterchangeDisk.cpp"
)

set_target_properties(FloppyLib PROPERTIES
	VERSION ${PROJECT_VERSION}
	PUBLIC_HEADER include/FloppyLib.h
)

target_include_directories(
	FloppyLib
	PUBLIC
		include
)

target_include_directories(
	FloppyLib
	PRIVATE
		src
)

#add_executable(tester src/FloppyLib.cpp)
#target_link_libraries(tester FloppyLib)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET FloppyLib PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
