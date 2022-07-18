cmake_minimum_required(VERSION 2.8.12)

project(MotherDuckExtension)

add_extension_definitions()

include_directories(include ../.. ../../third_party/httplib
                    ../../third_party/picohash ../parquet/include)

add_library(motherduck_extension STATIC s3fs.cpp motherduck.cpp crypto.cpp
        motherduck-extension.cpp)

build_loadable_extension(motherduck s3fs.cpp motherduck.cpp crypto.cpp
        motherduck-extension.cpp)

find_package(OpenSSL REQUIRED)
target_link_libraries(motherduck_loadable_extension ${OPENSSL_LIBRARIES})
install(
  TARGETS motherduck_extension
  EXPORT "${DUCKDB_EXPORT_SET}"
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR}")
