# X11 Test app

Приложение, которое при запуске создает одно окно,
заполненное цветом **#25854b**. По клику мыши, внутри 
окна, появляется треугольник, с одной 
из вершин в точке клика. При движении мыши, с 
зажатой левой клавишей, треугольник 
двигается вместе с курсором.

Приложение работает на ПК под управлением **LINUX**

Для реализации использованы следующие технологии:

* Linux
* C++
* Xlib
* OpenGL
* CMake

Для сборки проекта можно используется CMake.

Приложение имеет следующие зависимости:

* **libX11**:
  ```
  sudo apt update
  sudo apt install libx11-dev
  ```
* **libGL**:
  ```
  sudo apt-get update
  sudo apt-get install libgl-dev
  ```

### Инструкция по установке проекта
1) Необходимо создать папку,
   в которой будет собран проект
    ```
    mkdir <build_directory>
    ```
2) Далее, необходимо перейти в папку сборки
    ```
    cd <path_to_build_directory>
    ```
3) Запустите команду **cmake** для генерации
   **Makefile**
    ```
    cmake <path_to_source_directory>
    ```
4) Запустите команду **make** для сборки   
    ```
    make
    ```
