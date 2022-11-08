
set(CMAKE_CURRENT_BINARY_DIR "${CMAKE_ARGV3}")

message(STATUS "Packaging assets from assets/assets directory")

file(ARCHIVE_CREATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/bin/assets.data
        PATHS assets/assets/
        FORMAT zip
)

message(STATUS "Asset data created at ${CMAKE_CURRENT_BINARY_DIR}/bin/assets.data")