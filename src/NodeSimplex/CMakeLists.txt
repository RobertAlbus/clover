project(NodeSimplex)

add_library(${PROJECT_NAME}
INTERFACE
    basic
    delay
    envelope
    filter
    reduce
    stereo
    wavetable
)