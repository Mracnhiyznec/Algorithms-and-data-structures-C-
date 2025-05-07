import matplotlib.pyplot as plt
import numpy as np


def bresenham_x(x0, y0, x1, y1):
    if (y0 >= y1):
        add_y = -1
    else:
        add_y = 1
    if (x0 <= x1):
        add_x = 1
    else:
        add_x = -1
    error = 0
    to_add = abs(y0 - y1) + 1
    dots = []
    zap = abs(x1 - x0) + 1
    while x0 != x1 or y0 != y1:
        dots.append((x0, y0))
        x0 += add_x
        error += to_add
        if error >= (zap):
            y0 += add_y
            error -= (zap)
    dots.append((x0, y0))
    return dots

def bresenham_y(x0, y0, x1, y1):
    if (y0 >= y1):
        add_y = -1
    else:
        add_y = 1
    if (x0 <= x1):
        add_x = 1
    else:
        add_x = -1
    error = 0
    to_add = abs(x0 - x1) + 1
    dots = []
    zap = abs(y1 - y0) + 1
    while x0 != x1 or y0 != y1:
        dots.append((x0, y0))
        y0 += add_y
        error += to_add
        if error >= (zap):
            x0 += add_x
            error -= (zap)
    dots.append((x0, y0))
    return dots

def bressenhame_circle(xc, yc, r):
    dots = []
    x, y = 0, r
    F = 3 - 2 * r

    while x <= y:
        dots.extend([
            (xc + x, yc + y), (xc - x, yc + y),
            (xc + x, yc - y), (xc - x, yc - y),
            (xc + y, yc + x), (xc - y, yc + x),
            (xc + y, yc - x), (xc - y, yc - x)
        ])
        if F < 0:
            F += 4 * x + 6
        else:
            F += 4 * (x - y) + 10
            y -= 1
        x += 1
    return dots

def plot_bresenham(x0: int, y0: int, x1: int, y1: int):
    if (abs(x0-x1) >= abs(y0-y1)):
        lst = bresenham_x(x0, y0, x1, y1)
    else:
        lst = bresenham_y(x0,y0,x1,y1)
    print("Points:", lst)
    # Собираем все координаты
    xs = [x for x, y in lst]
    ys = [y for x, y in lst]

    # Определяем границы сетки
    min_x, max_x = min(xs), max(xs)
    min_y, max_y = min(ys), max(ys)

    # Создаём матрицу для отображения
    grid = np.zeros((max_y - min_y + 1, max_x - min_x + 1), dtype=int)

    # Заполняем матрицу
    for x, y in lst:
        grid[y - min_y, x - min_x] = 1  # инверсия Y для правильного отображения

    # Создаём фигуру
    plt.figure(figsize=(8, 8))

    # Отображаем матрицу как изображение
    plt.imshow(grid,
               cmap='binary',
               origin='lower',
               extent=[min_x - 0.5, max_x + 0.5, min_y - 0.5, max_y + 0.5])

    # Настройки внешнего вида
    plt.xticks(np.arange(min_x, max_x + 1))
    plt.yticks(np.arange(min_y, max_y + 1))
    plt.grid(which='both', color='gray', linestyle='-', linewidth=0.5)
    plt.gca().set_aspect('equal')
    plt.title(f'Bresenham line from ({x0}, {y0}) to ({x1}, {y1})')
    plt.xlabel('x'), plt.ylabel('y')
    plt.show()



def plot_bressenhame_circle(xc, yc, r):
    lst = bressenhame_circle(xc, yc, r)
    # Собираем все координаты
    xs = [x for x, y in lst]
    ys = [y for x, y in lst]

    # Определяем границы сетки
    min_x, max_x = min(xs), max(xs)
    min_y, max_y = min(ys), max(ys)

    # Создаём матрицу для отображения
    grid = np.zeros((max_y - min_y + 1, max_x - min_x + 1), dtype=int)

    # Заполняем матрицу
    for x, y in lst:
        grid[y - min_y, x - min_x] = 1  # инверсия Y для правильного отображения

    # Создаём фигуру
    plt.figure(figsize=(8, 8))

    # Отображаем матрицу как изображение
    plt.imshow(grid,
               cmap='binary',
               origin='lower',
               extent=[min_x - 0.5, max_x + 0.5, min_y - 0.5, max_y + 0.5])

    # Настройки внешнего вида
    plt.xticks(np.arange(min_x, max_x + 1))
    plt.yticks(np.arange(min_y, max_y + 1))
    plt.grid(which='both', color='gray', linestyle='-', linewidth=0.5)
    plt.gca().set_aspect('equal')
    plt.xlabel('x'), plt.ylabel('y')
    plt.show()


# Пример использования:
if __name__ == '__main__':
    type = int(input())
    if (type == 1):
        x0 = int(input("x0: "))
        y0 = int(input("y0: "))
        x1 = int(input("x1: "))
        y1 = int(input("y1: "))
        plot_bresenham(x0, y0, x1, y1)
    else:
        xc = int(input())
        yc = int(input())
        r = int(input())
        plot_bressenhame_circle(xc,yc,r)
