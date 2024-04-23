#ifndef MOVING_IMG_H
#define MOVING_IMG_H

#include "basics.h"
#include <stack>
#include <vector>
#include <iostream>
#include <queue>

// Clase que representa una imagen como una colección de 3 matrices siguiendo el
// esquema de colores RGB

class moving_image
{
private:
  unsigned char **red_layer;   // Capa de tonalidades rojas
  unsigned char **green_layer; // Capa de tonalidades verdes
  unsigned char **blue_layer;  // Capa de tonalidades azules
  std::stack<std::pair<char, int>> stackUndo;
  std::stack<std::pair<char, int>> stackRedo;
  std::queue<std::pair<char, int>> queueRepeatAll;
  int pixels;
  // Para evitar pushear letras al stack cuando se llama al undo
public:
  bool flag = true;
  // Constructor de la imagen. Se crea una imagen por defecto
  moving_image()
  {
    // Reserva de memoria para las 3 matrices RGB
    red_layer = new unsigned char *[H_IMG];
    green_layer = new unsigned char *[H_IMG];
    blue_layer = new unsigned char *[H_IMG];
    pixels = 0;
    for (int i = 0; i < H_IMG; i++)
    {
      red_layer[i] = new unsigned char[W_IMG];
      green_layer[i] = new unsigned char[W_IMG];
      blue_layer[i] = new unsigned char[W_IMG];
    }

    // Llenamos la imagen con su color de fondo
    for (int i = 0; i < H_IMG; i++)
      for (int j = 0; j < W_IMG; j++)
      {
        red_layer[i][j] = DEFAULT_R;
        green_layer[i][j] = DEFAULT_G;
        blue_layer[i][j] = DEFAULT_B;
      }

    // Dibujamos el objeto en su posición inicial
    for (int i = 0; i < 322; i++)
      for (int j = 0; j < 256; j++)
      {
        if (!s_R[i][j] && !s_G[i][j] && !s_B[i][j])
        {
          red_layer[INIT_Y + i][INIT_X + j] = DEFAULT_R;
          green_layer[INIT_Y + i][INIT_X + j] = DEFAULT_G;
          blue_layer[INIT_Y + i][INIT_X + j] = DEFAULT_B;
        }
        else
        {
          red_layer[INIT_Y + i][INIT_X + j] = s_R[i][j];
          green_layer[INIT_Y + i][INIT_X + j] = s_G[i][j];
          blue_layer[INIT_Y + i][INIT_X + j] = s_B[i][j];
        }
      }
  }

  // Destructor de la clase
  ~moving_image()
  {
    for (int i = 0; i < H_IMG; i++)
    {
      delete red_layer[i];
      delete green_layer[i];
      delete blue_layer[i];
    }

    delete red_layer;
    delete green_layer;
    delete blue_layer;
  }

  // Función utilizada para guardar la imagen en formato .png
  void draw(const char *nb)
  {
    _draw(nb);
  }
  void cantidadStack()
  {
    while (!queueRepeatAll.empty())
    {
      std::cout << queueRepeatAll.front().second;
      std::cout << queueRepeatAll.front().first;
      queueRepeatAll.pop();
    }
  }
  void llenarStack(char aux, int pixels, int countFunc)
  {
    for (int i = 0; i < countFunc; i++)
    {
      stackUndo.push({aux, pixels});
    }
  }
  void repeatAll()
  {
  }
  void repeat()
  {
    if (stackUndo.empty())
    {
      std::cout << "undomalo";
    }
    if (!stackUndo.empty() && stackUndo.top().first == 'd')
    {
      flag = true;
      int distance = stackUndo.top().second;
      move_right(distance);
      queueRepeatAll.push({'d', distance});
    }
    if (!stackUndo.empty() && stackUndo.top().first == 'u')
    {
      flag = true;
      int distance = stackUndo.top().second;
      move_up(distance);
      queueRepeatAll.push({'u', distance});
    }
    else if (!stackUndo.empty() && stackUndo.top().first == 'l')
    {
      flag = true;
      int distance = stackUndo.top().second;
      move_left(distance);
      queueRepeatAll.push({'l', distance});
    }
    else if (!stackUndo.empty() && stackUndo.top().first == 'b')
    {
      flag = true;
      int distance = stackUndo.top().second;
      move_down(distance);
      queueRepeatAll.push({'b', distance});
    }
    else if (!stackUndo.empty() && stackUndo.top().first == 'r')
    {
      flag = true;
      rotate();
      queueRepeatAll.push({'r', 0});
    }
  }
  void redo()
  {
    if (stackRedo.empty())
    {
      std::cout << "redomalo";
    }
    else if (!stackRedo.empty() && stackRedo.top().first == 'r')
    {
      flag = false;
      rotate();
      queueRepeatAll.push({'r', 0});
      stackRedo.pop();
    }
    else if (!stackRedo.empty() && stackRedo.top().first == 'l')
    {
      flag = false;
      int distance = stackRedo.top().second;
      move_left(distance);
      queueRepeatAll.push({'l', distance});
      stackRedo.pop();
    }
    else if (!stackRedo.empty() && stackRedo.top().first == 'd')
    {
      flag = false;
      int distance = stackRedo.top().second;
      move_right(distance);
      queueRepeatAll.push({'d', distance});
      stackRedo.pop();
    }
    else if (!stackRedo.empty() && stackRedo.top().first == 'u')
    {
      flag = false;
      int distance = stackRedo.top().second;
      move_up(distance);
      queueRepeatAll.push({'u', distance});
      stackRedo.pop();
    }
    else if (!stackRedo.empty() && stackRedo.top().first == 'b')
    {
      flag = false;
      int distance = stackRedo.top().second;
      move_down(distance);
      queueRepeatAll.push({'b', distance});
      stackRedo.pop();
    }
    flag = true;
  }
  void undo()
  {
    if (stackUndo.empty())
    {
      std::cout << "undomalo";
    }
    if (!stackUndo.empty() && stackUndo.top().first == 'd')
    {
      flag = false;
      int distance = stackUndo.top().second;
      move_left(distance);
      stackRedo.push({'d', distance});
      queueRepeatAll.push({'l', distance});
      stackUndo.pop();
    }
    if (!stackUndo.empty() && stackUndo.top().first == 'u')
    {
      flag = false;
      int distance = stackUndo.top().second;
      move_down(distance);
      stackRedo.push({'u', distance});
      queueRepeatAll.push({'b', distance});
      stackUndo.pop();
    }
    else if (!stackUndo.empty() && stackUndo.top().first == 'l')
    {
      flag = false;
      int distance = stackUndo.top().second;
      move_right(distance);
      stackRedo.push({'l', distance});
      queueRepeatAll.push({'d', distance});
      stackUndo.pop();
    }
    else if (!stackUndo.empty() && stackUndo.top().first == 'b')
    {
      flag = false;
      int distance = stackUndo.top().second;
      move_up(distance);
      stackRedo.push({'b', distance});
      queueRepeatAll.push({'u', distance});
      stackUndo.pop();
    }
    else if (!stackUndo.empty() && stackUndo.top().first == 'r')
    {
      flag = false;
      stackRedo.push({'r', 0});
      queueRepeatAll.push({'r', 0});
      stackUndo.pop();
      for (int i = 0; i < 3; i++)
      {
        rotate();
      }
    }
    flag = true;
  }
  void rotate()
  {
    int count = 0;
    count++;
    if (flag)
    {
      llenarStack('r', 0, count);
    }
    unsigned char tmp_layer[H_IMG][W_IMG];
    // Rotar la capa roja
    for (int i = 0; i < W_IMG; i++)
      for (int j = 0; j < H_IMG; j++)
        tmp_layer[i][j] = red_layer[j][H_IMG - 1 - i];

    for (int i = 0; i < W_IMG; i++)
      for (int j = 0; j < H_IMG; j++)
        red_layer[i][j] = tmp_layer[i][j];

    // Rotar la capa verde
    for (int i = 0; i < W_IMG; i++)
      for (int j = 0; j < H_IMG; j++)
        tmp_layer[i][j] = green_layer[j][H_IMG - 1 - i];

    for (int i = 0; i < H_IMG; i++)
      for (int j = 0; j < W_IMG; j++)
        green_layer[i][j] = tmp_layer[i][j];

    // Rotar capa azul
    for (int i = 0; i < W_IMG; i++)
      for (int j = 0; j < H_IMG; j++)
        tmp_layer[i][j] = blue_layer[j][H_IMG - 1 - i];

    for (int i = 0; i < H_IMG; i++)
      for (int j = 0; j < W_IMG; j++)
        blue_layer[i][j] = tmp_layer[i][j];
  }
  void move_up(int d)
  {
    char rotate = 'u';
    pixels = d;
    int count = 0;
    count++;
    if (flag)
    {
      llenarStack('u', pixels, count);
    }
    unsigned char tmp_layer[H_IMG][W_IMG];
    // Mover la capa roja
    for (int i = 0; i < W_IMG - d; i++)
      for (int j = 0; j < H_IMG; j++)
        tmp_layer[i][j] = red_layer[i + d][j];

    for (int i = 0; i < W_IMG; i++)
      for (int j = H_IMG - d, k = 0; j < H_IMG; j++, k++)
        tmp_layer[j][i] = red_layer[k][i];

    for (int i = 0; i < H_IMG; i++)
      for (int j = 0; j < W_IMG; j++)
        red_layer[i][j] = tmp_layer[i][j];

    // Mover la capa verde
    for (int i = 0; i < W_IMG - d; i++)
      for (int j = 0; j < H_IMG; j++)
        tmp_layer[i][j] = green_layer[i + d][j];

    for (int i = 0; i < W_IMG; i++)
      for (int j = H_IMG - d, k = 0; j < H_IMG; j++, k++)
        tmp_layer[j][i] = green_layer[k][i];

    for (int i = 0; i < H_IMG; i++)
      for (int j = 0; j < W_IMG; j++)
        green_layer[i][j] = tmp_layer[i][j];

    // Mover la capa azul
    for (int i = 0; i < W_IMG - d; i++)
      for (int j = 0; j < H_IMG; j++)
        tmp_layer[i][j] = blue_layer[i + d][j];

    for (int i = 0; i < W_IMG; i++)
      for (int j = H_IMG - d, k = 0; j < H_IMG; j++, k++)
        tmp_layer[j][i] = blue_layer[k][i];

    for (int i = 0; i < H_IMG; i++)
      for (int j = 0; j < W_IMG; j++)
        blue_layer[i][j] = tmp_layer[i][j];
  }

  void move_down(int d)
  {
    pixels = d;
    int count = 0;
    count++;
    if (flag)
    {
      llenarStack('b', pixels, count);
    }
    unsigned char tmp_layer[H_IMG][W_IMG];
    // Mover la capa roja
    for (int i = d; i < W_IMG; i++)
      for (int j = 0; j < H_IMG; j++)
        tmp_layer[i][j] = red_layer[i - d][j];

    for (int i = 0; i < W_IMG; i++)
      for (int j = 0, k = H_IMG - d; j < d; j++, k++)
        tmp_layer[j][i] = red_layer[k][i];

    for (int i = 0; i < H_IMG; i++)
      for (int j = 0; j < W_IMG; j++)
        red_layer[i][j] = tmp_layer[i][j];

    // Mover la capa verde
    for (int i = d; i < W_IMG; i++)
      for (int j = 0; j < H_IMG; j++)
        tmp_layer[i][j] = green_layer[i - d][j];

    for (int i = 0; i < W_IMG; i++)
      for (int j = 0, k = H_IMG - d; j < d; j++, k++)
        tmp_layer[j][i] = green_layer[k][i];

    for (int i = 0; i < H_IMG; i++)
      for (int j = 0; j < W_IMG; j++)
        green_layer[i][j] = tmp_layer[i][j];

    // Mover la capa azul
    for (int i = d; i < W_IMG; i++)
      for (int j = 0; j < H_IMG; j++)
        tmp_layer[i][j] = blue_layer[i - d][j];

    for (int i = 0; i < W_IMG; i++)
      for (int j = 0, k = H_IMG - d; j < d; j++, k++)
        tmp_layer[j][i] = blue_layer[k][i];

    for (int i = 0; i < H_IMG; i++)
      for (int j = 0; j < W_IMG; j++)
        blue_layer[i][j] = tmp_layer[i][j];
  }
  // Función que similar desplazar la imagen, de manera circular, d pixeles a la izquierda
  void move_right(int d)
  {
    pixels = d;
    int count = 0;
    count++;
    if (flag)
    {
      llenarStack('d', pixels, count);
    }
    unsigned char tmp_layer[H_IMG][W_IMG];

    // Mover la capa roja
    for (int i = 0; i < H_IMG; i++)
      for (int j = d; j < W_IMG; j++)
        tmp_layer[i][j] = red_layer[i][j - d];

    for (int i = 0; i < H_IMG; i++)
      for (int j = 0, k = W_IMG - d; j < d; j++, k++)
        tmp_layer[i][j] = red_layer[i][k];

    for (int i = 0; i < H_IMG; i++)
      for (int j = 0; j < W_IMG; j++)
        red_layer[i][j] = tmp_layer[i][j];

    // Mover la capa verde
    for (int i = 0; i < H_IMG; i++)
      for (int j = d; j < W_IMG; j++)
        tmp_layer[i][j] = green_layer[i][j - d];

    for (int i = 0; i < H_IMG; i++)
      for (int j = 0, k = W_IMG - d; j < d; j++, k++)
        tmp_layer[i][j] = green_layer[i][k];

    for (int i = 0; i < H_IMG; i++)
      for (int j = 0; j < W_IMG; j++)
        green_layer[i][j] = tmp_layer[i][j];

    // Mover la capa azul
    for (int i = 0; i < H_IMG; i++)
      for (int j = d; j < W_IMG; j++)
        tmp_layer[i][j] = blue_layer[i][j - d];

    for (int i = 0; i < H_IMG; i++)
      for (int j = 0, k = W_IMG - d; j < d; j++, k++)
        tmp_layer[i][j] = blue_layer[i][k];

    for (int i = 0; i < H_IMG; i++)
      for (int j = 0; j < W_IMG; j++)
        blue_layer[i][j] = tmp_layer[i][j];
  }
  void move_left(int d)
  {
    pixels = d;
    int count = 0;
    count++;
    if (flag)
    {
      llenarStack('l', pixels, count);
    }
    unsigned char tmp_layer[H_IMG][W_IMG];
    // Mover la capa roja
    for (int i = 0; i < H_IMG; i++)
      for (int j = 0; j < W_IMG - d; j++)
        tmp_layer[i][j] = red_layer[i][j + d];

    for (int i = 0; i < H_IMG; i++)
      for (int j = W_IMG - d, k = 0; j < W_IMG; j++, k++)
        tmp_layer[i][j] = red_layer[i][k];

    for (int i = 0; i < H_IMG; i++)
      for (int j = 0; j < W_IMG; j++)
        red_layer[i][j] = tmp_layer[i][j];

    // Mover la capa verde
    for (int i = 0; i < H_IMG; i++)
      for (int j = 0; j < W_IMG - d; j++)
        tmp_layer[i][j] = green_layer[i][j + d];

    for (int i = 0; i < H_IMG; i++)
      for (int j = W_IMG - d, k = 0; j < W_IMG; j++, k++)
        tmp_layer[i][j] = green_layer[i][k];

    for (int i = 0; i < H_IMG; i++)
      for (int j = 0; j < W_IMG; j++)
        green_layer[i][j] = tmp_layer[i][j];

    // Mover la capa azul
    for (int i = 0; i < H_IMG; i++)
      for (int j = 0; j < W_IMG - d; j++)
        tmp_layer[i][j] = blue_layer[i][j + d];

    for (int i = 0; i < H_IMG; i++)
      for (int j = W_IMG - d, k = 0; j < W_IMG; j++, k++)
        tmp_layer[i][j] = blue_layer[i][k];

    for (int i = 0; i < H_IMG; i++)
      for (int j = 0; j < W_IMG; j++)
        blue_layer[i][j] = tmp_layer[i][j];
  }

private:
  // Función privada que guarda la imagen en formato .png
  void _draw(const char *nb)
  {
    unsigned char *rgb = new unsigned char[H_IMG * W_IMG * 3], *p = rgb;
    unsigned x, y;

    // La imagen resultante tendrá el nombre dado por la variable 'nb'
    FILE *fp = fopen(nb, "wb");

    // Transformamos las 3 matrices en una arreglo unidimensional
    for (y = 0; y < H_IMG; y++)
      for (x = 0; x < W_IMG; x++)
      {
        *p++ = red_layer[y][x];   /* R */
        *p++ = green_layer[y][x]; /* G */
        *p++ = blue_layer[y][x];  /* B */
      }
    // La función svpng() transforma las 3 matrices RGB en una imagen PNG
    svpng(fp, W_IMG, H_IMG, rgb, 0);
    fclose(fp);
  }
};

#endif
