
set(INCLUDES ${ROOT_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/hipo/hipo4 ${CMAKE_SOURCE_DIR}/hipo/lib)
include_directories(${INCLUDES})
link_directories(${CMAKE_SOURCE_DIR}/hipo/lib)
set(CMAKE_VERBOSE_MAKEFILE ON)




#Analyze_HPSCosmicPMTs#############################################################################################
add_executable(Analyze_HPSCosmicPMTs.exe Analyze_HPSCosmicPMTs.cc)
target_link_libraries(Analyze_HPSCosmicPMTs.exe ${ROOT_LIBRARIES} Hipo4  -lpthread -lz -lm -ldl -lssl -lcrypto)
install( TARGETS Analyze_HPSCosmicPMTs.exe
		DESTINATION ${CMAKE_SOURCE_DIR}/bin )
#################################################################################################

#Analyze_Data#############################################################################################
add_executable(AnalyzeData.exe AnalyzeData.cc)
target_link_libraries(AnalyzeData.exe ${ROOT_LIBRARIES} Hipo4  -lpthread -lz -lm -ldl -lssl -lcrypto)
install( TARGETS AnalyzeData.exe
		DESTINATION ${CMAKE_SOURCE_DIR}/bin )
#################################################################################################

#AnalyzeSec6TOFTrg DOESNOT WORK AT THE MOMENT NEEDS PARTICLE LIBRARY#############################################################################################
#add_executable(AnalyzeSec6TOFTrg.exe AnalyzeSec6TOFTrg.cc)
#target_link_libraries(AnalyzeSec6TOFTrg.exe ${ROOT_LIBRARIES} Hipo4  -lpthread -lz -lm -ldl -lssl -lcrypto)
#install( TARGETS AnalyzeSec6TOFTrg.exe
#		DESTINATION ${CMAKE_SOURCE_DIR}/bin )
#################################################################################################

#CheckDecoding_CSCh#############################################################################################
add_executable(CheckDecoding_CSCh.exe CheckDecoding_CSCh.cc)
target_link_libraries(CheckDecoding_CSCh.exe ${ROOT_LIBRARIES} Hipo4  -lpthread -lz -lm -ldl -lssl -lcrypto)
install( TARGETS CheckDecoding_CSCh.exe
		DESTINATION ${CMAKE_SOURCE_DIR}/bin )
#################################################################################################

#CheckDecoding#############################################################################################
add_executable(CheckDecoding.exe CheckDecoding.cc)
target_link_libraries(CheckDecoding.exe ${ROOT_LIBRARIES} Hipo4  -lpthread -lz -lm -ldl -lssl -lcrypto)
install( TARGETS CheckDecoding.exe
		DESTINATION ${CMAKE_SOURCE_DIR}/bin )
#################################################################################################

#CheckTrgBits#############################################################################################
add_executable(CheckTrgBits.exe CheckTrgBits.cc)
target_link_libraries(CheckTrgBits.exe ${ROOT_LIBRARIES} Hipo4  -lpthread -lz -lm -ldl -lssl -lcrypto)
install( TARGETS CheckTrgBits.exe
		DESTINATION ${CMAKE_SOURCE_DIR}/bin )
#################################################################################################

#Skim_Coincidence#############################################################################################
add_executable(Skim_Coincidence.exe Skim_Coincidence.cc)
target_link_libraries(Skim_Coincidence.exe ${ROOT_LIBRARIES} Hipo4  -lpthread -lz -lm -ldl -lssl -lcrypto)
install( TARGETS Skim_Coincidence.exe
		DESTINATION ${CMAKE_SOURCE_DIR}/bin )
#################################################################################################

#Skim_Cross#############################################################################################
add_executable(Skim_Cross.exe Skim_Cross.cc)
target_link_libraries(Skim_Cross.exe ${ROOT_LIBRARIES} Hipo4  -lpthread -lz -lm -ldl -lssl -lcrypto)
install( TARGETS Skim_Cross.exe
		DESTINATION ${CMAKE_SOURCE_DIR}/bin )
#################################################################################################
