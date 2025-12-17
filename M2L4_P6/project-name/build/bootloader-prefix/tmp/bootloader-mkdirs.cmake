# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/bibha/esp/v6.0/esp-idf/components/bootloader/subproject"
  "/home/bibha/Desktop/test/esp/tracedemo/tracedemo_3/project-name/build/bootloader"
  "/home/bibha/Desktop/test/esp/tracedemo/tracedemo_3/project-name/build/bootloader-prefix"
  "/home/bibha/Desktop/test/esp/tracedemo/tracedemo_3/project-name/build/bootloader-prefix/tmp"
  "/home/bibha/Desktop/test/esp/tracedemo/tracedemo_3/project-name/build/bootloader-prefix/src/bootloader-stamp"
  "/home/bibha/Desktop/test/esp/tracedemo/tracedemo_3/project-name/build/bootloader-prefix/src"
  "/home/bibha/Desktop/test/esp/tracedemo/tracedemo_3/project-name/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/bibha/Desktop/test/esp/tracedemo/tracedemo_3/project-name/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/bibha/Desktop/test/esp/tracedemo/tracedemo_3/project-name/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
