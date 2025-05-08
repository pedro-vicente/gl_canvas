cmake_minimum_required(VERSION 3.17)
set(CMAKE_BUILD_TYPE Debug)
project (gl_canvas)

set(CMAKE_CXX_STANDARD 17)
option(BUILD_STATIC "BUILD_STATIC" OFF)
if (MSVC)
  message(STATUS "Static build: " ${BUILD_STATIC})
  if(BUILD_STATIC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  endif()
endif()
message(STATUS "Source directory is " ${CMAKE_SOURCE_DIR})
message(STATUS "Build directory is " ${CMAKE_CURRENT_BINARY_DIR})


#//////////////////////////
# Linux/Mac compiling flags and needed libraries 
# build/ext/wxWidgets-3.2.8/wx-config --cppflags
# build/ext/wxWidgets-3.2.8/wx-config --libs

#//////////////////////////
# ext(ernal) dependencies 
# dependencies are dealt with:
# add_subdirectory(source_dir): Used to add a subdirectory to the build. There is also a CMakeLists.txt file in the source_dir. 
# include_directories(dir): Add the given directories to those the compiler uses to search for include files.
#//////////////////////////


#//////////////////////////
#ODBC Linux/Mac
#//////////////////////////

if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

if(APPLE)
  message(STATUS "Mac OS build...")
endif()

#//////////////////////////
# wxWidgets library locations 
#//////////////////////////

if (MSVC)
  include_directories(${CMAKE_SOURCE_DIR}/build/ext/wxWidgets-3.2.8/lib/vc_x64_lib/mswud)
  link_directories(${CMAKE_SOURCE_DIR}/build/ext/wxWidgets-3.2.8/lib/vc_x64_lib)
elseif(APPLE)
  include_directories(${CMAKE_SOURCE_DIR}/build/ext/wxWidgets-3.2.8/lib/wx/include/osx_cocoa-unicode-static-3.1)
  link_directories(${CMAKE_SOURCE_DIR}/build/ext/wxWidgets-3.2.8/lib)
  add_definitions(-DWXUSINGDLL -D__WXOSX_COCOA__ -D__WXMAC__ -D__WXOSX__)
elseif(LINUX)
  add_definitions(-D__WXGTK3__)
  add_definitions(-D__WXGTK__)
  link_directories(${CMAKE_SOURCE_DIR}/build/ext/wxWidgets-3.2.8/lib)
  include_directories(${CMAKE_SOURCE_DIR}/build/ext/wxWidgets-3.2.8/lib/wx/include/gtk3-unicode-static-3.2)
endif()

#//////////////////////////
# wxWidgets source
#//////////////////////////

include_directories(${CMAKE_SOURCE_DIR}/ext/wxWidgets-3.2.8/include)


#//////////////////////////
#source common
#//////////////////////////

set(src_extra ${src_extra})
if (MSVC)
  set(src_extra ${src_extra} ${CMAKE_SOURCE_DIR}/src/sample.rc)
elseif(APPLE AND NOT IPHONE)
  set(src_extra ${src_extra} ${CMAKE_SOURCE_DIR}/build/ext/wxWidgets-3.2.8/src/osx/carbon/wxmac.icns)
endif()

#//////////////////////////
#executables 
#//////////////////////////

add_executable(minimal WIN32 MACOSX_BUNDLE src/minimal.cc ${src_extra})

#//////////////////////////
#LINUX
#order of the link libraries matters; pthread dl ${ODBC_LIBRARY} must be last
#//////////////////////////

if(LINUX)
  set(lib_dep ${lib_dep} pthread dl)
  find_program(LSB_RELEASE_EXEC lsb_release)
  execute_process(COMMAND ${LSB_RELEASE_EXEC} -is OUTPUT_VARIABLE LSB_RELEASE_ID_SHORT OUTPUT_STRIP_TRAILING_WHITESPACE)
  message(STATUS "Building in " ${LSB_RELEASE_ID_SHORT})
endif()

#//////////////////////////
#link with libraries
#lib_dep contains a cascade definition of all the libraries needed to link
#//////////////////////////

set(lib_dep ${lib_dep})

if (MSVC)
  set(lib_dep ${lib_dep} wxbase32ud wxmsw32ud_core wxmsw32ud_webview wxbase32ud_net wxpngd wxzlibd wxregexud wxmsw32ud_html)
  set(lib_dep ${lib_dep} odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib ws2_32.lib)
endif()

if (LINUX)
  set(lib_dep ${lib_dep} wx_gtk3u_xrc-3.2 wx_gtk3u_qa-3.2 wx_baseu_net-3.2 wx_gtk3u_html-3.2 wx_gtk3u_core-3.2 wx_baseu_xml-3.2 wx_baseu-3.2)
  set(lib_dep ${lib_dep} SM ICE X11 Xext Xtst xkbcommon)
  set(lib_dep ${lib_dep} gtk-3 gdk-3 pangocairo-1.0 pango-1.0 harfbuzz atk-1.0 cairo-gobject cairo gdk_pixbuf-2.0)
  set(lib_dep ${lib_dep} gio-2.0 gobject-2.0 glib-2.0)
  set(lib_dep ${lib_dep} jpeg png z expat pcre2-32 c dl)
endif()

if(APPLE)
  set(lib_dep ${lib_dep} wx_osx_cocoau_xrc-3.2 wx_osx_cocoau_qa-3.2 wx_baseu_net-3.2 wx_osx_cocoau_html-3.2 wx_osx_cocoau_core-3.2)
  set(lib_dep ${lib_dep} wx_baseu_xml-3.2 wx_baseu-3.2 wx_osx_cocoau_webview-3.2)
  set(lib_dep ${lib_dep} wx_osx_cocoau_core-3.2 wxregexu-3.2 wxjpeg-3.2 wxpng-3.2 wxtiff-3.2 z iconv)
  find_library(COREFOUNDATION CoreFoundation)
  if (NOT COREFOUNDATION)
      message(FATAL_ERROR "CoreFoundation not found")
  endif()
  find_library(SECURITY Security)
  find_library(CARBON Carbon)
  find_library(COCOA Cocoa)
  find_library(IOKIT IOKit)
  find_library(WEBKIT WebKit)
  set(lib_dep ${lib_dep} ${COREFOUNDATION} ${SECURITY} ${CARBON} ${COCOA} ${IOKIT} ${WEBKIT})
endif()

target_link_libraries(minimal ${lib_dep})
if (MSVC)
  set_target_properties(minimal PROPERTIES LINK_FLAGS "/SubSystem:\"Windows\"" VS_DPI_AWARE "ON")
endif()

if(APPLE)
  set(VERSION "1.0")
  set(COPYRIGHT "Me")
  if(NOT IPHONE)
    set_target_properties(minimal PROPERTIES
      MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/build/ext/wxWidgets-3.2.8/samples/Info.plist.in"
      RESOURCE "${CMAKE_SOURCE_DIR}/build/ext/wxWidgets-3.2.8/src/osx/carbon/wxmac.icns")
  endif()
  set_target_properties(minimal PROPERTIES
    MACOSX_BUNDLE_ICON_FILE wxmac.icns
    MACOSX_BUNDLE_LONG_VERSION_STRING "${VERSION}"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION}"
    MACOSX_BUNDLE_VERSION "${VERSION}"
    MACOSX_BUNDLE_COPYRIGHT "${COPYRIGHT}"
    MACOSX_BUNDLE_GUI_IDENTIFIER "org.wxwidgets.me"
    )
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT minimal)

