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


---

# Отчёт по домашнему заданию

В рамках домашнего задания необходимо настроить пакетирование для приложения `solver` (из предыдущей лабораторной работы) с помощью CPack и GitHub Actions.

### Структура проекта

В репозиторий добавлены папки из `lab03`:
- `formatter_lib/`
- `formatter_ex_lib/`
- `solver_lib/`
- `solver_application/`

Корневой `CMakeLists.txt` обновлён:

```cmake
cmake_minimum_required(VERSION 3.10)
project(examples)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PRINT_VERSION_MAJOR 1)
set(PRINT_VERSION_MINOR 0)
set(PRINT_VERSION_PATCH 0)
set(PRINT_VERSION_TWEAK 0)
set(PRINT_VERSION ${PRINT_VERSION_MAJOR}.${PRINT_VERSION_MINOR}.${PRINT_VERSION_PATCH}.${PRINT_VERSION_TWEAK})
set(PRINT_VERSION_STRING "v${PRINT_VERSION}")

add_subdirectory(formatter_lib)
add_subdirectory(formatter_ex_lib)
add_subdirectory(solver_lib)
add_subdirectory(solver_application)

install(TARGETS solver_app
        DESTINATION bin
        COMPONENT applications)

include(CPackConfig.cmake)
```

Для каждого подмодуля созданы свои CMakeLists.txt (с версией CMake 3.10 для совместимости с macOS).

Настройка CPack
Файл CPackConfig.cmake дополнен для создания пакетов с приложением solver:

```cmake
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_CONTACT nkuzin279@gmail.com)
set(CPACK_PACKAGE_VERSION_MAJOR ${PRINT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PRINT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PRINT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION_TWEAK ${PRINT_VERSION_TWEAK})
set(CPACK_PACKAGE_VERSION ${PRINT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "solver library")

set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md)

set(CPACK_RPM_PACKAGE_NAME "solver")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "solver")
set(CPACK_RPM_PACKAGE_RELEASE 1)
set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")

set(CPACK_DEBIAN_PACKAGE_NAME "libsolver-dev")
set(CPACK_DEBIAN_PACKAGE_PREDEPENDS "cmake >= 3.0")
set(CPACK_DEBIAN_PACKAGE_RELEASE 1)

set(CPACK_GENERATOR "DEB;RPM;TGZ;ZIP")
include(CPack)
```

## GitHub Actions
Создан файл .github/workflows/cpack.yml, который:

-запускается при создании тега v*,

-собирает проект,

-создаёт пакеты DEB, RPM, TGZ,

-загружает их в Release на GitHub.

Упрощённый вариант workflow (только Linux):

```yaml
name: build and release

on:
  push:
    tags: [ "v*" ]
  workflow_dispatch:

permissions:
  contents: write

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - run: cmake -H. -B build
      - run: cmake --build build
      - uses: actions/upload-artifact@v4
        with:
          name: Applications
          path: ./build/solver_application/solver_app

  test:
    needs: build
    runs-on: ubuntu-latest
    steps:
      - uses: actions/download-artifact@v4
        with:
          name: Applications
          path: ./apps
      - run: chmod +x ./apps/solver_app
      - run: echo "1 5 -6" | ./apps/solver_app

  create-binary-packages:
    needs: test
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - run: sudo apt-get update && sudo apt-get install -y rpm
      - run: cmake -H. -B build
      - run: cmake --build build --config Release
      - run: |
          cd build
          cpack -G DEB -C Release
          cpack -G RPM -C Release
          cpack -G TGZ -C Release
      - uses: softprops/action-gh-release@v2
        with:
          files: |
            build/*.deb
            build/*.rpm
            build/*.tar.gz
        env:
          GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
```

Результат
После пуша тега v1.0.0 GitHub Actions автоматически создаёт релиз с пакетами:

*.deb — для Debian/Ubuntu,

*.rpm — для Red Hat/Fedora,

*.tar.gz — универсальный архив.

Пакеты содержат исполняемый файл solver_app, который решает квадратное уравнение.

Домашнее задание выполнено полностью.
