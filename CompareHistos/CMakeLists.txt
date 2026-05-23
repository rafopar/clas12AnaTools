set(INCLUDES
  ${ROOT_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/CompareHistos)
include_directories(${INCLUDES})

link_directories(${CMAKE_BINARY_DIR}/lib)
set(CMAKE_VERBOSE_MAKEFILE ON)

ROOT_GENERATE_DICTIONARY(G__CompareHistosGUI CompareHistosGUI.h LINKDEF LinkDef.h)

################################  Adding Executables  ##########################################
add_executable(CompareHistosGUI CompareHistosMain.cc CompareHistosGUI.cc G__CompareHistosGUI.cxx)

set_target_properties(CompareHistosGUI PROPERTIES OUTPUT_NAME "CompareHistosGUI.exe")
target_link_libraries(CompareHistosGUI ${ROOT_LIBRARIES} ROOT::RIO ROOT::Hist ROOT::Graf ROOT::Gpad ROOT::Gui)
#################################################################################################

################################ Installing Executable targets #############################
install( TARGETS CompareHistosGUI 
  DESTINATION ${CMAKE_INSTALL_PREFIX}/CompareHistos/bin )
