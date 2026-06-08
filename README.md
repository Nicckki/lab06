# Лабораторная работа №6

Данная лабораторная работа посвящена изучению средств пакетирования на примере CPack.

## Tutorial

Склонируем репозиторий 5 лабораторной работы и привяжем его к новому репозиторию.

```bash
git clone https://github.com/Nicckki/lab05 projects/lab06
cd projects/lab06
git remote remove origin
git remote add origin https://github.com/Nicckki/lab06
git branch -M main
```

обавим в CMakeLists.txt переменные, отвечающие за версию пакета, с помощью gsed (в Linux gsed — это алиас для sed).

```bash
gsed -i '/project(print)/a\set(PRINT_VERSION_MAJOR 0)' CMakeLists.txt
gsed -i '/project(print)/a\set(PRINT_VERSION_MINOR 1)' CMakeLists.txt
gsed -i '/project(print)/a\set(PRINT_VERSION_PATCH 0)' CMakeLists.txt
gsed -i '/project(print)/a\set(PRINT_VERSION_TWEAK 0)' CMakeLists.txt
gsed -i '/project(print)/a\set(PRINT_VERSION\
  ${PRINT_VERSION_MAJOR}.${PRINT_VERSION_MINOR}.${PRINT_VERSION_PATCH}.${PRINT_VERSION_TWEAK})' CMakeLists.txt
gsed -i '/project(print)/a\set(PRINT_VERSION_STRING "v${PRINT_VERSION}")' CMakeLists.txt
```

Посмотрим, что изменилось:

```bash
git diff
```

<details> 
  <summary>Вывод git diff</summary>
  
    diff
    diff --git a/CMakeLists.txt b/CMakeLists.txt
    index ... --- a/CMakeLists.txt
    +++ b/CMakeLists.txt
    @@ -7,6 +7,13 @@ option(BUILD_EXAMPLES "Build examples" OFF)
     project(print)
    +set(PRINT_VERSION_MAJOR 0)
    +set(PRINT_VERSION_MINOR 1)
    +set(PRINT_VERSION_PATCH 0)
    +set(PRINT_VERSION_TWEAK 0)
    +set(PRINT_VERSION
    +  ${PRINT_VERSION_MAJOR}.${PRINT_VERSION_MINOR}.${PRINT_VERSION_PATCH}.${PRINT_VERSION_TWEAK})
    +set(PRINT_VERSION_STRING "v${PRINT_VERSION}")
    
     add_library(print STATIC ${CMAKE_CURRENT_SOURCE_DIR}/sources/print.cpp)
</details>

Теперь создадим файлы, необходимые для описания пакета.

```bash
touch DESCRIPTION && nano DESCRIPTION
# Введите описание: "Static C++ library for formatted printing"

touch ChangeLog.md
export DATE="`LANG=en_US date +'%a %b %d %Y'`"
cat > ChangeLog.md <<EOF
* ${DATE} Nicckki <nkuzin279@gmail.com> 0.1.0.0
- Initial RPM release
EOF
```

Создадим конфигурационный файл CPackConfig.cmake:

```bash
cat > CPackConfig.cmake <<EOF
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_CONTACT nkuzin279@gmail.com)
set(CPACK_PACKAGE_VERSION_MAJOR \${PRINT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR \${PRINT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH \${PRINT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION_TWEAK \${PRINT_VERSION_TWEAK})
set(CPACK_PACKAGE_VERSION \${PRINT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_FILE \${CMAKE_CURRENT_SOURCE_DIR}/DESCRIPTION)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "static C++ library for printing")

set(CPACK_RESOURCE_FILE_LICENSE \${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README \${CMAKE_CURRENT_SOURCE_DIR}/README.md)

set(CPACK_RPM_PACKAGE_NAME "print-devel")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "print")
set(CPACK_RPM_CHANGELOG_FILE \${CMAKE_CURRENT_SOURCE_DIR}/ChangeLog.md)
set(CPACK_RPM_PACKAGE_RELEASE 1)

set(CPACK_DEBIAN_PACKAGE_NAME "libprint-dev")
set(CPACK_DEBIAN_PACKAGE_PREDEPENDS "cmake >= 3.0")
set(CPACK_DEBIAN_PACKAGE_RELEASE 1)

include(CPack)
EOF
```

Подключим этот файл к основному CMakeLists.txt:

```bash
cat >> CMakeLists.txt <<EOF
include(CPackConfig.cmake)
EOF
```

Закоммитим изменения и создадим тег:

```bash
git add .
git commit -m "added cpack config"
git tag v0.1.0.0
git push origin main --tags
```

Соберём проект и создадим архив:

```bash
cmake -H. -B build -DCPACK_GENERATOR="TGZ"
cmake --build build --target package
mkdir artifacts
mv build/*.tar.gz artifacts
tree artifacts
```

<details>  
  <summary>Вывод tree artifacts</summary>


      artifacts/
      └── print-0.1.0.0-Linux.tar.gz
      
      0 directories, 1 file
</details>

Архив создан — туториал успешно выполнен.
