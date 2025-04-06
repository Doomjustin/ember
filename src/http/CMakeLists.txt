add_library(ember_net_http
    STATIC
        http_request.cpp
        http_request.h
        http_response.cpp
        http_response.h
        http_server.cpp
        http_server.h
        method.h
        status_code.h
        version.h
)

add_library(ember::http ALIAS ember_net_http)

target_link_libraries(ember_net_http 
    PUBLIC 
        ember::base
        ember::tcp
)