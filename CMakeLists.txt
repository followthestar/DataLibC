cmake_minimum_required(VERSION 3.29)
project(DataLibC C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/build
        CACHE PATH ${PROJECT_SOURCE_DIR}/cache)
set(BUILD_SHARED_LIBS ON)


find_library(WINSDK_XAUDIO2_LIBRARY NAMES XAudio2_8)

add_library(DataLibC SHARED
        DataLibC/Tool/ExportModuleMacro.h
        DataLibC/Tool/Misc.c
        DataLibC/DS/Hash/Hash.c
        DataLibC/DS/Hash/hash_table.c
        DataLibC/DS/List/DynamicArray.c
        DataLibC/DS/List/LinkedList.c
        DataLibC/DS/List/SortedVector.c
        DataLibC/DS/List/StaticArray.c
        DataLibC/DS/Map/SortedMap.c
        DataLibC/DS/Pair/Pair.c
        DataLibC/DS/Queue/Queue.c
        DataLibC/DS/Set/HashSet.c
        DataLibC/DS/Set/SortedSet.c
        DataLibC/DS/Set/TreeSet.c
        DataLibC/DS/Stack/Stack.c
        DataLibC/DS/String/String.c
        DataLibC/DS/Algorithm.c
        DataLibC/DS/Allocation.c
        DataLibC/DS/DefaultOperation.c
        DataLibC/DS/Iterator.c
        DataLibC/DS/TypeEnum.c
        DataLibC/IO/Input/Input.c
        DataLibC/IO/Input/Input.h
        DataLibC/IO/Input/InputState.c
        DataLibC/LA/Math/MathTool.c
        DataLibC/LA/Matrix/Matrix3x3.c
        DataLibC/LA/Matrix/Matrix4x4.c
        DataLibC/LA/Vector/Vector2.c
        DataLibC/LA/Vector/Vector3.c
        DataLibC/LA/Vector/Vector4.c
        DataLibC/DS/Tree/tree.c
        DataLibC/GFX/Texture/Texture.c
        DataLibC/GFX/Texture/Texture.h
        DataLibC/GFX/Texture/image_bmp.c
        DataLibC/GFX/Texture/image_bmp.h
        DataLibC/GFX/Texture/image_jpg.c
        DataLibC/GFX/Texture/image_jpg.h
        DataLibC/GFX/Texture/image_png.c
        DataLibC/GFX/Texture/image_png.h
        DataLibC/SFX/Audio.c
        DataLibC/SFX/Audio.h
        DataLibC/SFX/AudioState.c
        DataLibC/SFX/AudioState.h
        DataLibC/SFX/AudioClip.c
        DataLibC/SFX/AudioClip.h
        DataLibC/SFX/AudioSource.c
        DataLibC/SFX/AudioSource.h
        DataLibC/Compression/ZLib/ZLib.c
        DataLibC/Compression/ZLib/ZLib.h
        DataLibC/Compression/Compression.c
        DataLibC/Compression/Compression.h
        DataLibC/Compression/BitReader.c
        DataLibC/Compression/BitReader.h
        DataLibC/Compression/HaffmanTree.c
        DataLibC/Compression/HaffmanTree.h
        DataLibC/Compression/BitWriter.c
        DataLibC/Compression/BitWriter.h
        DataLibC/Objective/VObject.c
        DataLibC/Objective/VObject.h
        DataLibC/Objective/VType.c
        DataLibC/Objective/VType.h
        DataLibC/Anim/AnimValue/AnimInt.c
        DataLibC/Anim/AnimValue/AnimInt.h
        DataLibC/Anim/Interface/IAnimated.h
        DataLibC/Anim/AnimValue/AnimFloat.c
        DataLibC/Anim/AnimValue/AnimFloat.h
        DataLibC/Anim/AnimValue/AnimBool.c
        DataLibC/Anim/AnimValue/AnimBool.h
        DataLibC/Anim/Interface/IAnimated.c
        DataLibC/IO/Serialization/Serializable/ISerializable.c
        DataLibC/IO/Serialization/Serializable/ISerializable.h
        DataLibC/IO/Serialization/Transfer/Interface/ITransfer.h
        DataLibC/IO/TextStream/Reader.h
        DataLibC/IO/TextStream/StringReader.c
        DataLibC/IO/TextStream/StringReader.h
        DataLibC/IO/TextStream/Reader.c
        DataLibC/IO/TextStream/FileReader.c
        DataLibC/IO/TextStream/FileReader.h
        DataLibC/IO/TextStream/BufferedReader.c
        DataLibC/IO/TextStream/BufferedReader.h
        DataLibC/IO/TextStream/Reader_InternalMacro.h
        DataLibC/IO/Serialization/Transfer/JsonReader.c
        DataLibC/IO/Serialization/Transfer/JsonReader.h
        DataLibC/IO/Serialization/Transfer/Interface/ITransfer.c
        DataLibC/IO/Serialization/Transfer/Internal/Json.c
        DataLibC/IO/Serialization/Transfer/Internal/Json.h
        DataLibC/IO/TextStream/Writer.c
        DataLibC/IO/TextStream/Writer.h
        DataLibC/IO/Serialization/Transfer/JsonWriter.c
        DataLibC/IO/Serialization/Transfer/JsonWriter.h
        DataLibC/DS/Map/TreeMap.c
        DataLibC/DS/Map/TreeMap.h
        DataLibC/Tool/Color.h
        DataLibC/Anim/Animation/Keyframe.c
        DataLibC/Anim/Animation/Keyframe.h
        DataLibC/Anim/Animation/AnimationCurve.c
        DataLibC/Anim/Animation/AnimationCurve.h
        DataLibC/Allocator/MemoryPool.c
        DataLibC/Allocator/MemoryPool.h
        DataLibC/Allocator/DynamicMemoryPool.c
        DataLibC/Allocator/DynamicMemoryPool.h
        DataLibC/Anim/Animation/Animation.c
        DataLibC/Anim/Animation/Animation.h
        DataLibC/Anim/Animation/AnimationClip.c
        DataLibC/Anim/Animation/AnimationClip.h
        DataLibC/Anim/Animation/Internal/AnimationState.c
        DataLibC/Anim/Animation/Internal/AnimationState.h
        DataLibC/Anim/Animation/Internal/BoundCurve.c
        DataLibC/Anim/Animation/Internal/BoundCurve.h
        DataLibC/Anim/Animation/Internal/WrapCurve.h
        DataLibC/Physics/Collider/Collider2D.h
        DataLibC/Physics/Collider/BoxCollider2D.c
        DataLibC/Physics/Collider/BoxCollider2D.h
        DataLibC/Physics/Collider/Internal/Collider2DImpl.c
        DataLibC/Physics/Collider/Internal/Collider2DImpl.h
        DataLibC/Allocator/ObjectPool.c
        DataLibC/Allocator/ObjectPool.h
        DataLibC/Anim/AnimValue/AnimVector2.c
        DataLibC/Anim/AnimValue/AnimVector2.h
        DataLibC/Coroutine/Coroutine.c
        DataLibC/Coroutine/Coroutine.h
        DataLibC/Pointer/Pointer.c
        DataLibC/Pointer/Pointer.h
        DataLibC/Pointer/Pointer_Internal.c
        DataLibC/Pointer/Pointer_Internal.h
        DataLibC/Delegate/Message_Internal.c
        DataLibC/Delegate/Message_Internal.h
        DataLibC/Delegate/Message.c
        DataLibC/Delegate/Message.h
        DataLibC/Tool/Magic.h
        DataLibC/Net/Socket/Socket.c
        DataLibC/Net/Socket/Socket.h
        DataLibC/Net/Error/NetError.c
        DataLibC/Net/Error/NetError.h
        DataLibC/Net/Error/NetError_Internal.h
        DataLibC/Net/Socket/Socket_Internal.h
        DataLibC/Net/Socket/Socket_Internal.c
        DataLibC/Net/Error/NetError_Internal.c
        DataLibC/Net/IPAddress/IPAddress.c
        DataLibC/Net/IPAddress/IPAddress.h
        DataLibC/Net/IPAddress/IPAddress_Internal.h
        DataLibC/IO/Print.c
        DataLibC/IO/Print.h
        DataLibC/Net/Net.h
        DataLibC/Thread/ThreadPool.c
        DataLibC/Thread/ThreadPool.h
        DataLibC/Thread/ConcurrentQueue.c
        DataLibC/Thread/ConcurrentQueue.h
        DataLibC/GC/GC.c
        DataLibC/GC/GC.h
        DataLibC/Thread/Task.c
        DataLibC/Thread/Task.h
        DataLibC/Thread/Task_Internal.h
        DataLibC/Net/Socket/UdpServer.c
        DataLibC/Net/Socket/UdpServer.h
        DataLibC/Net/Socket/TcpServer.c
        DataLibC/Net/Socket/TcpServer.h
        DataLibC/Thread/Mutex.c
        DataLibC/Thread/Mutex.h
        DataLibC/GFX/Camera/Camera.c
        DataLibC/GFX/Camera/Camera.h
        DataLibC/System/Console/Console.c
        DataLibC/System/Console/Console.h
        DataLibC/Platform/Windows/WinConsole.c
        DataLibC/Tool/Color32.h
        DataLibC/Physics/RayCast/Ray.c
        DataLibC/Physics/RayCast/Ray.h
        DataLibC/Physics/RayCast/RayCast.c
        DataLibC/Physics/RayCast/RayCast.h
        DataLibC/Physics/Collider/Collider.c
        DataLibC/Physics/Collider/Collider.h
        DataLibC/Thread/Tread.c
        DataLibC/Thread/Tread.h
        DataLibC/DS/BitSet/BitSet.c
        DataLibC/DS/BitSet/BitSet.h
        DataLibC/Exception/Exception.c
        DataLibC/Exception/Exception.h
        DataLibC/Objective/VType_Internal.h
        DataLibC/Objective/VMethod.c
        DataLibC/Objective/VMethod.h
)


target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>)

target_link_libraries(${PROJECT_NAME} PRIVATE ${WINSDK_XAUDIO2_LIBRARY} ws2_32)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY DataLibC/
        DESTINATION include/${PROJECT_NAME}
        FILES_MATCHING PATTERN "*.h"
)

add_definitions(-DENABLE_EXPORT)
add_definitions(-DEXPORT_MODULE)