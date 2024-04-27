#ifndef MOVING_IMG_H
#define MOVING_IMG_H

#include "basics.h"
#include <stack>
#include <vector>
#include <iostream>
#include <queue>
#include <cstdio>
#include <string.h>

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
    while (!stackUndo.empty())
    {
      std::cout << stackUndo.top().first;
      std::cout << stackUndo.top().second << std::endl;
      stackUndo.pop();
    }
  }
  void cantidadQueue()
  {
    while (!queueRepeatAll.empty())
    {
      std::cout << queueRepeatAll.front().first;
      std::cout << queueRepeatAll.front().second;
      queueRepeatAll.pop();
    }
  }
  void llenarStack(char aux, int pixels)
  {
    stackUndo.push({aux, pixels});
  }
  void llenarQueue(char aux, int pixels)
  {
    queueRepeatAll.push({aux, pixels});
  }
  void repeat_all()
  {
    moving_image im;
    im.setCoordsImageDefault();
    im.draw("imagenfuente.png");
    while (!queueRepeatAll.empty())
    {
      flag = false;
      int count = queueRepeatAll.size();
      count--;

      std::string chain = std::to_string(count);
      std::string filename = "imagen" + chain + ".png";

      char instruccion = queueRepeatAll.front().first;
      int distance = queueRepeatAll.front().second;
      switch (instruccion)
      {
      case 'b':
        im.move_down(distance);
        im.draw(filename.c_str());
        break;
      case 'l':
        im.move_left(distance);
        im.draw(filename.c_str());
        break;
      case 'r':
        im.rotate();
        im.draw(filename.c_str());
        break;
      case 'R':
        im.undoRotate();
        im.draw(filename.c_str());
        break;
      case 'u':
        im.move_up(distance);
        im.draw(filename.c_str());
        break;
      case 'd':
        im.move_right(distance);
        im.draw(filename.c_str());
        break;
      }
      queueRepeatAll.pop();
    }
  }
  void repeat()
  {
    if (stackUndo.empty())
    {
      std::cout << "repeat malo";
    }
    else
    {
      flag = false;
      char instruccion = stackUndo.top().first;
      int distance = stackUndo.top().second;
      switch (instruccion)
      {
      case 'u':
        move_up(distance);
        break;
      case 'b':
        move_down(distance);
        break;
      case 'l':
        move_left(distance);
        break;
      case 'r':
        rotate();
        break;
      case 'd':
        move_right(distance);
        break;
      }
      llenarQueue(instruccion, distance);
    }
  }
  void redo()
  {
    if (stackRedo.empty())
    {
      std::cout << "redo malo";
    }
    else
    {
      flag = false;
      char instruccion = stackRedo.top().first;
      int distance = stackRedo.top().second;
      switch (instruccion)
      {
      case 'u':
        move_up(distance);
        break;
      case 'b':
        move_down(distance);
        break;
      case 'l':
        move_left(distance);
        break;
      case 'r':
        rotate();
        break;
      case 'd':
        move_right(distance);
        break;
      }
      llenarQueue(instruccion, pixels);
      stackRedo.pop();
      flag = true;
    }
  }
  void undo()
  {
    if (stackUndo.empty())
    {
      std::cout << "Undo malo";
    }
    else
    {
      flag = false;
      char instruccion = stackUndo.top().first;
      int distance = stackUndo.top().second;

      switch (instruccion)
      {
      case 'd':
        move_left(distance);
        llenarQueue('l', pixels);
        break;
      case 'l':
        move_right(distance);
        llenarQueue('d', pixels);
        break;
      case 'u':
        move_down(distance);
        llenarQueue('b', pixels);
        break;
      case 'b':
        move_up(distance);
        llenarQueue('u', pixels);
        break;
      case 'r':
        llenarQueue('R', pixels);
        for (int i = 0; i < 3; i++)
        {
          rotate();
        }
        break;
      }
      stackRedo.push({instruccion, distance});
      stackUndo.pop();
      flag = true;
    }
  }
  void rotate()
  {
    if (flag)
    {
      llenarQueue('r', 0);
      llenarStack('r', 0);
      std::stack<std::pair<char, int>> redo;
      stackRedo = redo;
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
    pixels = d;
    if (flag)
    {
      llenarQueue('u', pixels);
      llenarStack('u', pixels);
      std::stack<std::pair<char, int>> redo;
      stackRedo = redo;
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
    if (flag)
    {
      llenarQueue('b', pixels);
      llenarStack('b', pixels);
      std::stack<std::pair<char, int>> redo;
      stackRedo = redo;
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
    if (flag)
    {
      llenarQueue('d', pixels);
      llenarStack('d', pixels);
      std::stack<std::pair<char, int>> redo;
      stackRedo = redo;
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
    if (flag)
    {
      llenarQueue('l', pixels);
      llenarStack('l', pixels);
      std::stack<std::pair<char, int>> redo;
      stackRedo = redo;
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
  void setCoordsImageDefault()
  {
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
  void undoRotate()
  {
    flag = false;
    for (int i = 0; i < 3; i++)
    {
      rotate();
    }
  }
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
