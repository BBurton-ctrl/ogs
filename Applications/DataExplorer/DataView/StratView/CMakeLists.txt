set(SOURCES StratBar.cpp StratScene.cpp StratView.cpp StratWindow.cpp)

set(HEADERS StratBar.h StratScene.h StratView.h StratWindow.h)

set(SOURCE_DIR_REL ${CMAKE_CURRENT_SOURCE_DIR}/../../../..)
set(GUI_SOURCE_DIR_REL ${CMAKE_CURRENT_SOURCE_DIR}/../..)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${SOURCE_DIR_REL}/BaseLib
                    ${SOURCE_DIR_REL}/GeoLib
                    ${SOURCE_DIR_REL}/MathLib
                    ${GUI_SOURCE_DIR_REL}/Base)

file(GLOB_RECURSE UI_FILES CONFIGURE_DEPENDS *.ui)
source_group("UI Files" FILES ${UI_FILES})

add_library(QtStratView ${SOURCES} ${HEADERS} ${UIS})
if(BUILD_SHARED_LIBS)
    install(TARGETS QtStratView LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

target_link_libraries(QtStratView
                      PRIVATE BaseLib
                              GeoLib
                              DataHolderLib
                              QtBase
                              Qt5::Gui)
add_autogen_include(QtStratView)

set_property(TARGET QtStratView PROPERTY FOLDER "DataExplorer")

if(OGS_USE_PCH)
    cotire(QtStratView)
endif()
