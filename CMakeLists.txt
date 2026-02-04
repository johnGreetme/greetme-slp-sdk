cmake_minimum_required(VERSION 3.10)
project(greetme_slp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Create the static library
add_library(greetme_slp STATIC
    src/slp_manager.cpp
    src/tee_stub.cpp
)

# Create the demo app executable
add_executable(demo_app examples/demo_app.cpp)

# Link the library to the demo app
target_link_libraries(demo_app PRIVATE greetme_slp)

# Optional: Add a test target if needed (placeholder for now)
add_custom_target(run_demo
    COMMAND demo_app
    DEPENDS demo_app
    COMMENT "Running the demo app..."
)
