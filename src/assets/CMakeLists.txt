
set(CMAKE_CURRENT_BINARY_DIR "${CMAKE_ARGV3}")

file(ARCHIVE_CREATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/bin/assets.data
        PATHS assets/assets/
        FORMAT zip
        VERBOSE
)