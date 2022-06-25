cmake_minimum_required(VERSION 2.6)
if(MSVC)
	set(CMAKE_GENERATOR_PLATFORM Win32)
endif()
project(GLWRAP)

include_directories(
	src
	include
	lib
)

add_library(glwrap
	src/glwrap/glwrap.h
	src/glwrap/NonCopyable.h

	src/glwrap/FileManager.h
	src/glwrap/FileManager.cpp

	src/glwrap/Context.h
	src/glwrap/Context.cpp
	
	src/glwrap/Texture.h
	src/glwrap/Texture.cpp
	
	src/glwrap/RenderTexture.h
	src/glwrap/RenderTexture.cpp
	src/glwrap/DepthBuffer.h
	src/glwrap/DepthBuffer.cpp
	src/glwrap/DepthCube.h
	src/glwrap/DepthCube.cpp
	src/glwrap/GBuffer.h
	src/glwrap/GBuffer.cpp
	
	src/glwrap/TriFace.h
	src/glwrap/TriFace.cpp

	src/glwrap/VertexBuffer.h
	src/glwrap/VertexBuffer.cpp
	src/glwrap/VertexArray.h
	src/glwrap/VertexArray.cpp

	src/glwrap/ArrayPiece.h
	src/glwrap/ArrayPiece.cpp
	src/glwrap/BufferArray.h
	src/glwrap/BufferArray.cpp

	src/glwrap/ShaderProgram.h
	src/glwrap/ShaderProgram.cpp
	src/glwrap/ViewingFrustum.h
	src/glwrap/ViewingFrustum.cpp
	src/glwrap/TextureUniforms.h

	src/glwrap/Model3D.h
	src/glwrap/PBRUniforms.h
	src/glwrap/PBRUniforms.cpp
	
	src/glwrap/Animation.h
	src/glwrap/Animation.cpp

	src/glwrap/ObjAnimation.h
	src/glwrap/ObjAnimation.cpp
	src/glwrap/ObjFrame.h
	src/glwrap/ObjFrame.cpp
	src/glwrap/ObjPart.h
	src/glwrap/ObjPart.cpp
	src/glwrap/ObjMtlModel.h
	src/glwrap/ObjMtlModel.cpp

	src/glwrap/Material.h
	src/glwrap/Material.cpp

	src/glwrap/ModelJoint.h
	src/glwrap/ModelJoint.cpp
	src/glwrap/GltfParse.h
	src/glwrap/GltfModel.h
	src/glwrap/GltfModel.cpp

	src/glwrap/ModelAnimation.h
	src/glwrap/ModelAnimation.cpp

	src/glwrap/stb_image.h
)

if(MSVC)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	target_link_libraries(glwrap glew32 SDL2 opengl32 OpenAL32)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	target_link_libraries(glwrap SDL2)
else()
	target_link_libraries(glwrap GLEW SDL2 GL)
endif()