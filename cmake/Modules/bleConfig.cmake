INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_BLE ble)

FIND_PATH(
    BLE_INCLUDE_DIRS
    NAMES ble/api.h
    HINTS $ENV{BLE_DIR}/include
        ${PC_BLE_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    BLE_LIBRARIES
    NAMES gnuradio-ble
    HINTS $ENV{BLE_DIR}/lib
        ${PC_BLE_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BLE DEFAULT_MSG BLE_LIBRARIES BLE_INCLUDE_DIRS)
MARK_AS_ADVANCED(BLE_LIBRARIES BLE_INCLUDE_DIRS)

