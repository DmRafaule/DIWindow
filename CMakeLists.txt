cmake_minimum_required(VERSION 3.5)
project(DIWindow DESCRIPTION "Window creator and manager" LANGUAGES CXX)

file( GLOB SOURCES **.cpp )

SetFlags()
SetLibraries( glfw GL DIDebuger )
CreateLibrary( ${PROJECT_NAME} )