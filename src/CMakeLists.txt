cmake_minimum_required(VERSION 3.25)
project(smartCalc2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(include)

set(CMAKE_PREFIX_PATH "/home/ruslan/Qt/6.4.2/gcc_64/lib/cmake/")
find_package(Qt6 COMPONENTS Core Widgets PrintSupport REQUIRED)
find_package(GTest REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/gui)

add_compile_options(-Werror -Wextra -Wall)

add_executable(smartCalc2
        sources/main.cc
        include/calc.h sources/calc.cc sources/calc-insertion.cc
        include/graph.h sources/graph.cc
        include/controller.h sources/controller.cc
        include/model.h sources/model.cc sources/model-financial.cc
        include/qcustomplot.h sources/qcustomplot.cpp
        include/credit.h sources/credit.cc
        include/deposit.h sources/deposit.cc
        gui/rs.qrc)

target_link_libraries(smartCalc2 Qt6::Widgets Qt6::PrintSupport)

add_executable(model_test
        include/model.h sources/model.cc sources/model-financial.cc
        tests/evaluation.cc tests/financial.cc tests/tests.h)

target_compile_options(model_test PRIVATE --coverage)

target_link_libraries(model_test GTest::gtest_main gcov)