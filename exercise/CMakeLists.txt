add_library(db.engine STATIC)

add_library(db::engine ALIAS db.engine)

target_sources(db.engine
    PUBLIC FILE_SET CXX_MODULES
    FILES
        meta_command.cppm
        statement.cppm
)

target_link_libraries(db.engine PUBLIC ember::dependency)

target_compile_features(db.engine PUBLIC cxx_std_23)


add_executable(db.server db_server.cpp)

target_link_libraries(db.server
    PRIVATE
        ember::dependency
        db::engine
)