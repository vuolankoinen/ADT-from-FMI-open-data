cmake_minimum_required(VERSION 2.8.4)
project(juuri)

# Minne valmiit menee: ${juuri_BINARY_DIR}
# Mist ettimiset alkaa: ${juuri_SOURCE_DIR}

SET(LIBRARY_OUTPUT_PATH ${juuri_BINARY_DIR}/../data/lib) #Toimisko kirjoitussuojauksesta huolimatta?
message("Library output path:" ${LIBRARY_OUTPUT_PATH})

include_directories(${juuri_SOURCE_DIR}/poco-1.7.3/Foundation/include)
include_directories(${juuri_SOURCE_DIR}/poco-1.7.3/Net/include)
include_directories(${juuri_SOURCE_DIR}/poco-1.7.3/Util/include)
#link_directories(${juuri_BINARY_DIR}/lib)

add_subdirectory(poco-1.7.3)
add_subdirectory(src)
#add_dependencies(Foundation Util)
#add_dependencies(Util Net)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++") # Staattisiksi