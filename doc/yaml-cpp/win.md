# windows 上编译 yaml-cpp


## Debug 模式

1. 测试时，需要将 yaml-cpp 的源码下载到本地，然后使用 cmake 进行编译。并且也需要编译为Debug模式，这样才能在调试时，查看源码。
2. 为了方便，可以将 yaml-cpp 编译后的文件，安装到 OSGeo4W 的目录下，这样就可以在 OSGeo4W 的环境下，使用 yaml-cpp 了。

```shell
D:\iProject\cpath\yaml-cpp>cmake -S . -B build -DCMAKE_INSTALL_PREFIX=D:/iProject/cpath/OSGeo4W/apps/yaml-cpp
-- Building for: Visual Studio 17 2022
CMake Deprecation Warning at CMakeLists.txt:2 (cmake_minimum_required):
  Compatibility with CMake < 3.5 will be removed from a future version of
  CMake.

  Update the VERSION argument <min> value or use a ...<max> suffix to tell
  CMake that the project does not need compatibility with older versions.


-- Selecting Windows SDK version 10.0.22621.0 to target Windows 10.0.22631.
-- The CXX compiler identification is MSVC 19.42.34435.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.42.34433/bin/Hostx64/x64/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - not found
-- Found Threads: TRUE
CMake Deprecation Warning at test/gtest-1.10.0/CMakeLists.txt:4 (cmake_minimum_required):
  Compatibility with CMake < 3.5 will be removed from a future version of
  CMake.

  Update the VERSION argument <min> value or use a ...<max> suffix to tell
  CMake that the project does not need compatibility with older versions.


-- The C compiler identification is MSVC 19.42.34435.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.42.34433/bin/Hostx64/x64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
CMake Deprecation Warning at test/gtest-1.10.0/googlemock/CMakeLists.txt:45 (cmake_minimum_required):
  Compatibility with CMake < 3.5 will be removed from a future version of
  CMake.

  Update the VERSION argument <min> value or use a ...<max> suffix to tell
  CMake that the project does not need compatibility with older versions.


CMake Deprecation Warning at test/gtest-1.10.0/googletest/CMakeLists.txt:56 (cmake_minimum_required):
  Compatibility with CMake < 3.5 will be removed from a future version of
  CMake.

  Update the VERSION argument <min> value or use a ...<max> suffix to tell
  CMake that the project does not need compatibility with older versions.


CMake Warning (dev) at test/gtest-1.10.0/googletest/cmake/internal_utils.cmake:243 (find_package):
  Policy CMP0148 is not set: The FindPythonInterp and FindPythonLibs modules
  are removed.  Run "cmake --help-policy CMP0148" for policy details.  Use
  the cmake_policy command to set the policy and suppress this warning.

Call Stack (most recent call first):
  test/gtest-1.10.0/googletest/CMakeLists.txt:91 (include)
This warning is for project developers.  Use -Wno-dev to suppress it.

-- Found PythonInterp: D:/iProject/cpath/OSGeo4W/bin/python.exe (found version "3.12.7")
-- Configuring done (14.7s)
-- Generating done (0.4s)
-- Build files have been written to: D:/iProject/cpath/yaml-cpp/build

D:\iProject\cpath\yaml-cpp>
D:\iProject\cpath\yaml-cpp>
D:\iProject\cpath\yaml-cpp>
D:\iProject\cpath\yaml-cpp>
D:\iProject\cpath\yaml-cpp>
D:\iProject\cpath\yaml-cpp>
D:\iProject\cpath\yaml-cpp>
D:\iProject\cpath\yaml-cpp>
D:\iProject\cpath\yaml-cpp>cmake --build build --config Debug --target install
适用于 .NET Framework MSBuild 版本 17.12.12+1cce77968

  Checking File Globs
  1>Checking Build System
  Building Custom Rule D:/iProject/cpath/yaml-cpp/test/gtest-1.10.0/googlemock/CMakeLists.txt
  gtest-all.cc
  gmock-all.cc
  正在生成代码...
  gmock.vcxproj -> D:\iProject\cpath\yaml-cpp\build\lib\Debug\gmockd.lib
  Building Custom Rule D:/iProject/cpath/yaml-cpp/test/gtest-1.10.0/googlemock/CMakeLists.txt
  gtest-all.cc
  gmock-all.cc
  gmock_main.cc
  正在生成代码...
  gmock_main.vcxproj -> D:\iProject\cpath\yaml-cpp\build\lib\Debug\gmock_maind.lib
  Building Custom Rule D:/iProject/cpath/yaml-cpp/test/gtest-1.10.0/googletest/CMakeLists.txt
  gtest-all.cc
  gtest.vcxproj -> D:\iProject\cpath\yaml-cpp\build\lib\Debug\gtestd.lib
  Building Custom Rule D:/iProject/cpath/yaml-cpp/test/gtest-1.10.0/googletest/CMakeLists.txt
  gtest_main.cc
  gtest_main.vcxproj -> D:\iProject\cpath\yaml-cpp\build\lib\Debug\gtest_maind.lib
  Building Custom Rule D:/iProject/cpath/yaml-cpp/CMakeLists.txt
  graphbuilder.cpp
  graphbuilderadapter.cpp
  binary.cpp
  convert.cpp
  depthguard.cpp
  directives.cpp
  emit.cpp
  emitfromevents.cpp
  emitter.cpp
  emitterstate.cpp
  emitterutils.cpp
  exceptions.cpp
  exp.cpp
  memory.cpp
  node.cpp
  node_data.cpp
  nodebuilder.cpp
  nodeevents.cpp
  null.cpp
  ostream_wrapper.cpp
  正在生成代码...
  正在编译...
  parse.cpp
  parser.cpp
  regex_yaml.cpp
  scanner.cpp
  scanscalar.cpp
  scantag.cpp
  scantoken.cpp
  simplekey.cpp
  singledocparser.cpp
  stream.cpp
  tag.cpp
  正在生成代码...
  yaml-cpp.vcxproj -> D:\iProject\cpath\yaml-cpp\build\Debug\yaml-cppd.lib
  Building Custom Rule D:/iProject/cpath/yaml-cpp/util/CMakeLists.txt
  parse.cpp
  yaml-cpp-parse.vcxproj -> D:\iProject\cpath\yaml-cpp\build\util\Debug\parse.exe
  Building Custom Rule D:/iProject/cpath/yaml-cpp/util/CMakeLists.txt
  read.cpp
  yaml-cpp-read.vcxproj -> D:\iProject\cpath\yaml-cpp\build\util\Debug\read.exe
  Building Custom Rule D:/iProject/cpath/yaml-cpp/util/CMakeLists.txt
  sandbox.cpp
  yaml-cpp-sandbox.vcxproj -> D:\iProject\cpath\yaml-cpp\build\util\Debug\sandbox.exe
  Building Custom Rule D:/iProject/cpath/yaml-cpp/test/CMakeLists.txt
  emitter_test.cpp
  encoding_test.cpp
  error_messages_test.cpp
  gen_emitter_test.cpp
  handler_spec_test.cpp
  handler_test.cpp
  load_node_test.cpp
D:\iProject\cpath\yaml-cpp\test\integration\load_node_test.cpp(1,1): warning C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失 [D:\iProject\cpath\
yaml-cpp\build\test\yaml-cpp-tests.vcxproj]
  node_spec_test.cpp
  main.cpp
  node_test.cpp
D:\iProject\cpath\yaml-cpp\test\node\node_test.cpp(549,19): warning C4305: “初始化”: 从“double”到“float”截断 [D:\iProject\cpath\yaml-cpp\build\test\yaml-cpp-tests
.vcxproj]
  ostream_wrapper_test.cpp
  parser_test.cpp
  regex_test.cpp
  正在生成代码...
  yaml-cpp-tests.vcxproj -> D:\iProject\cpath\yaml-cpp\build\test\Debug\yaml-cpp-tests.exe
  Building Custom Rule D:/iProject/cpath/yaml-cpp/CMakeLists.txt
  1>
  -- Install configuration: "Debug"
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/lib/yaml-cppd.lib
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/anchor.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/binary.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/contrib
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/contrib/anchordict.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/contrib/graphbuilder.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/depthguard.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/dll.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/emitfromevents.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/emitter.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/emitterdef.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/emittermanip.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/emitterstyle.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/eventhandler.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/exceptions.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/mark.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/node
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/node/convert.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/node/detail
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/node/detail/impl.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/node/detail/iterator.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/node/detail/iterator_fwd.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/node/detail/memory.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/node/detail/node.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/node/detail/node_data.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/node/detail/node_iterator.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/node/detail/node_ref.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/node/emit.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/node/impl.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/node/iterator.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/node/node.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/node/parse.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/node/ptr.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/node/type.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/noexcept.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/null.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/ostream_wrapper.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/parser.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/stlemitter.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/traits.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/yaml-cpp/yaml.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/share/cmake/yaml-cpp/yaml-cpp-targets.cmake
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/share/cmake/yaml-cpp/yaml-cpp-targets-debug.cmake
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/share/cmake/yaml-cpp/yaml-cpp-config.cmake
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/share/cmake/yaml-cpp/yaml-cpp-config-version.cmake
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/share/pkgconfig/yaml-cpp.pc
  -- Up-to-date: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/gmock-actions.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/gmock-cardinalities.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/gmock-function-mocker.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/gmock-generated-actions.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/gmock-generated-actions.h.pump
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/gmock-generated-function-mockers.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/gmock-generated-function-mockers.h.pump
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/gmock-generated-matchers.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/gmock-generated-matchers.h.pump
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/gmock-matchers.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/gmock-more-actions.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/gmock-more-matchers.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/gmock-nice-strict.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/gmock-spec-builders.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/gmock.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/internal
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/internal/custom
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/internal/custom/gmock-generated-actions.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/internal/custom/gmock-generated-actions.h.pump
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/internal/custom/gmock-matchers.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/internal/custom/gmock-port.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/internal/custom/README.md
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/internal/gmock-internal-utils.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/internal/gmock-port.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gmock/internal/gmock-pp.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/lib/gmockd.lib
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/lib/gmock_maind.lib
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/lib/gmockd.pdb
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/lib/gmock_maind.pdb
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/lib/pkgconfig/gmock.pc
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/lib/pkgconfig/gmock_main.pc
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/lib/cmake/GTest/GTestTargets.cmake
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/lib/cmake/GTest/GTestTargets-debug.cmake
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/lib/cmake/GTest/GTestConfigVersion.cmake
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/lib/cmake/GTest/GTestConfig.cmake
  -- Up-to-date: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/gtest-death-test.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/gtest-matchers.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/gtest-message.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/gtest-param-test.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/gtest-printers.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/gtest-spi.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/gtest-test-part.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/gtest-typed-test.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/gtest.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/gtest_pred_impl.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/gtest_prod.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/internal
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/internal/custom
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/internal/custom/gtest-port.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/internal/custom/gtest-printers.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/internal/custom/gtest.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/internal/custom/README.md
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/internal/gtest-death-test-internal.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/internal/gtest-filepath.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/internal/gtest-internal.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/internal/gtest-param-util.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/internal/gtest-port-arch.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/internal/gtest-port.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/internal/gtest-string.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/internal/gtest-type-util.h
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/include/gtest/internal/gtest-type-util.h.pump
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/lib/gtestd.lib
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/lib/gtest_maind.lib
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/lib/gtestd.pdb
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/lib/gtest_maind.pdb
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/lib/pkgconfig/gtest.pc
  -- Installing: D:/iProject/cpath/OSGeo4W/apps/yaml-cpp/lib/pkgconfig/gtest_main.pc
```