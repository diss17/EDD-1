/* Compilación: g++ -g -o img test.cpp
 * Ejecución: ./img
 *
 * Luego de la ejecución se generarán 3 imagines: imagen.png,
 * imagen_desplazada_500.png e imagen_desplazada_600.png
 */

#include "moving_image.h"
#include <windows.h> // para sleep (linux). Usar  #include<windows.h> para Windows

int main() {
  moving_image im;
  im.draw("source.png");
  im.move_down(400);
  im.draw("down400.png");
  im.move_left(200);
  im.draw("left200.png");
  im.undo();
  im.draw("undoleft200.png");
  im.undo();
  im.draw("undodown400.png");
  /* NOTA 1: Si usan el mismo nombre para las imágenes, entonces cada llamada al
  método draw() sobreescribirá a la imagen */

  /* NOTA 2: Si usan un ciclo while(1) y usan el mismo nombre de imagen,
  entonces se puede generar un efecto de desplazamiento continuo (algo así como
  un video =D ) */
  return 0;
}
