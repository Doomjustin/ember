add_library(ember_http
    STATIC
        http_request.cpp
        http_request.h
        http_response.cpp
        http_response.h
        http_server.cpp
        http_server.h
)

add_library(ember::http ALIAS ember_http)

target_link_libraries(ember_http 
    PUBLIC 
        ember::base
        ember::tcp
)