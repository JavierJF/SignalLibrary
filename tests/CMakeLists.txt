IF(BUILD_TESTS AND GTEST_FOUND)

   INCLUDE_DIRECTORIES(SYSTEM ${GTEST_INCLUDE_DIRS})
   INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/tests/include)

   LINK_LIBRARIES(${GTEST_BOTH_LIBRARIES} ${PROJECT_NAME})

   FILE(GLOB unit_srcs  "unit/*.cpp"
                        "unit/*.h")

   ADD_EXECUTABLE(TestsUnit ${unit_srcs} tests_main.cpp)

   # Build Performance tests
   if (Qt5Widgets_FOUND)
           INCLUDE_DIRECTORIES(SYSTEM ${Qt5Widgets_INCLUDES})

           FILE(GLOB performance_srcs    "src/*.cpp"
                                         "include/*.hpp")

           ADD_EXECUTABLE(TestPerformance ${performance_srcs} performance/PerformanceTests.cpp tests_main.cpp)
           QT5_USE_MODULES(TestPerformance Widgets)
   endif()

ELSEIF(NOT GTEST_FOUND)

   MESSAGE(WARNING "GTest is not installed")

ENDIF()  
