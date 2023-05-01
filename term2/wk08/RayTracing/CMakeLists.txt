include_directories("include")

add_library(RayTracing OBJECT
        "include/MF.h"
        "include/point.h"
        "include/ray.h"
        "include/object.h"
        "include/geometry.h"
        "include/physics.h"
        "include/mirror.h"
        "include/screen.h"
        "include/space.h"
        #"include/RayTracing.h"
        
        "src/point.cpp"
        "src/ray.cpp"
        "src/geometry.cpp"
        "src/physics.cpp"
        "src/mirror.cpp"
        "src/screen.cpp"
        "src/space.cpp")