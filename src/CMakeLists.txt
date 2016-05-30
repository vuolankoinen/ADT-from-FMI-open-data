cmake_minimum_required(VERSION 2.6)
aux_source_directory(. SRC_LIST)

project(ml)

include_directories(${juuri_SOURCE_DIR}/poco-1.7.3/Foundation/include)
message("Inkluusiohakemisto 1: " ${juuri_SOURCE_DIR}/poco-1.7.3/Foundation/include)
include_directories(${juuri_SOURCE_DIR}/poco-1.7.3/Net/include)
include_directories(${juuri_SOURCE_DIR}/poco-1.7.3/Util/include)

add_executable(ml_exec ${SRC_LIST})
add_dependencies(ml_exec Foundation)
add_dependencies(ml_exec Net)
add_dependencies(ml_exec Util)

set_target_properties(ml_exec PROPERTIES LINK_SEARCH_START_STATIC 1) # Staattiseksi
set_target_properties(ml_exec PROPERTIES LINK_SEARCH_END_STATIC 1) # Staattiseksi

#Onko noi turhia:
target_link_libraries(ml_exec :${juuri_BINARY_DIR}/lib/libPocoFoundation.a)
target_link_libraries(ml_exec :${juuri_BINARY_DIR}/lib/libPocoNet.a) 
target_link_libraries(ml_exec :${juuri_BINARY_DIR}/lib/libPocoUtil.a)
#target_link_libraries(ml_exec :${juuri_BINARY_DIR}/lib/libPocoFoundation.so)  
#target_link_libraries(ml_exec :${juuri_BINARY_DIR}/lib/libPocoNet.so) 
#target_link_libraries(ml_exec :${juuri_BINARY_DIR}/lib/libPocoUtil.so)
#target_link_libraries(ml_exec :${juuri_BINARY_DIR}/lib/libPocoFoundation.so.43)  
#target_link_libraries(ml_exec :${juuri_BINARY_DIR}/lib/libPocoNet.so.43) 
#target_link_libraries(ml_exec :${juuri_BINARY_DIR}/lib/libPocoUtil.so.43)