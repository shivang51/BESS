set(PROJECT_NAME LibBessScene)
set(CMAKE_CXX_STANDARD 20)

set(External
    "../external/glad/src/glad.c"
    "../external/stb_image/src/stb_image.cpp"
)

source_group("External" FILES ${External})

set(Header_Files
"include/entity/empty_entity.h"
"include/entity/entity.h"
"include/transform/transform_2d.h"
"include/events/entity_event.h"
"include/events/event.h"
"include/events/event_type.h"
"include/api.h"
"include/camera.h"
"include/scene_context.h"
"include/renderer/renderer.h"
"include/renderer/font.h"
"include/renderer/gl/texture.h"
"include/renderer/gl/vertex.h"
"include/renderer/gl/shader.h"
"include/renderer/gl/framebuffer.h"
"include/renderer/gl/primitive_type.h"
"include/renderer/gl/fb_attachment.h"
"include/renderer/gl/vao.h"
"include/renderer/gl/gl_wrapper.h"
"include/renderer/gl/utils.h"
)

source_group("Headers" FILES ${Header_Files})

set(Source_Files 
"src/entity/empty_entity.cpp"
"src/entity/entity.cpp"
"src/transform/transform_2d.cpp"
"src/api.cpp"
"src/camera.cpp"
"src/scene_context.cpp"
"src/events/entity_event.cpp"
"src/events/event.cpp"
"src/renderer/font.cpp"
"src/renderer/gl/gl_wrapper.cpp"
"src/renderer/gl/shader.cpp"
"src/renderer/gl/texture.cpp"
"src/renderer/gl/framebuffer.cpp"
"src/renderer/gl/vao.cpp"
"src/renderer/gl/fb_attachment.cpp"
"src/renderer/renderer.cpp"
)

set(ALL_FILES
    ${External}
    ${Header_Files}
    ${Shaders}
    ${Source_Files}
)

add_compile_definitions(DLL_EXPORT)
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../external/uuid;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../external/freetype/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../external/glm;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../external/json;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../external/glad/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../external/opengl_es3.0/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../external/stb_image/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
    )
endif()

# Dependencies

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64" AND NOT UNIX)
    target_link_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_SOURCE_DIR}/external/freetype/libs/windows"
    )
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "opengl32;"
        "gdi32;"
        "freetype;"
    )
else()
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "GL;"
        "freetype;"
    )
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")