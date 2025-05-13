import matplotlib.pyplot as plt
import numpy as np


def bresenham_x(x0, y0, x1, y1):
    add_y = -1 if y0 >= y1 else 1
    add_x = 1 if x0 <= x1 else -1
    error = 0
    to_add = abs(y0 - y1) + 1
    dots = []
    zap = abs(x1 - x0) + 1
    while x0 != x1 or y0 != y1:
        dots.append((x0, y0))
        x0 += add_x
        error += to_add
        if error >= zap:
            y0 += add_y
            error -= zap
    dots.append((x0, y0))
    return dots


def bresenham_y(x0, y0, x1, y1):
    add_y = -1 if y0 >= y1 else 1
    add_x = 1 if x0 <= x1 else -1
    error = 0
    to_add = abs(x0 - x1) + 1
    dots = []
    zap = abs(y1 - y0) + 1
    while x0 != x1 or y0 != y1:
        dots.append((x0, y0))
        y0 += add_y
        error += to_add
        if error >= zap:
            x0 += add_x
            error -= zap
    dots.append((x0, y0))
    return dots


def bresenham_circle(xc, yc, r):
    dots = []
    x, y = 0, r
    F = 1 - r
    while x <= y:
        dots.extend([
            (xc + x, yc + y), (xc - x, yc + y),
            (xc + x, yc - y), (xc - x, yc - y),
            (xc + y, yc + x), (xc - y, yc + x),
            (xc + y, yc - x), (xc - y, yc - x)
        ])
        if F < 0:
            F += 2 * x + 3
        else:
            y -= 1
            F += 2 * (x - y) + 5
        x += 1
    return dots



def plot_bresenham(x0: int, y0: int, x1: int, y1: int):
    lst = bresenham_x(x0, y0, x1, y1) if abs(x0 - x1) >= abs(y0 - y1) else bresenham_y(x0, y0, x1, y1)
    xs, ys = zip(*lst)

    grid = np.zeros((max(ys) - min(ys) + 1, max(xs) - min(xs) + 1), dtype=int)
    for x, y in lst:
        grid[y - min(ys), x - min(xs)] = 1

    plt.figure(figsize=(8, 8))
    plt.imshow(grid,
               cmap='binary',
               origin='lower',
               extent=[min(xs) - 0.5, max(xs) + 0.5, min(ys) - 0.5, max(ys) + 0.5],
               label='Bresenham')

    plt.plot([x0, x1], [y0, y1], linestyle='--', linewidth=1.5, label='Ровная линия')

    plt.xticks(np.arange(min(xs), max(xs) + 1))
    plt.yticks(np.arange(min(ys), max(ys) + 1))
    plt.grid(which='both', linestyle='-', linewidth=0.4)
    plt.gca().set_aspect('equal')
    plt.legend()
    plt.title(f'Line from ({x0}, {y0}) to ({x1}, {y1})')
    plt.xlabel('x'), plt.ylabel('y')
    plt.show()


def plot_bresenham_circle(xc: int, yc: int, r: int):
    lst = bresenham_circle(xc, yc, r)
    xs, ys = zip(*lst)

    grid = np.zeros((max(ys) - min(ys) + 1, max(xs) - min(xs) + 1), dtype=int)
    for x, y in lst:
        grid[y - min(ys), x - min(xs)] = 1

    plt.figure(figsize=(8, 8))
    plt.imshow(grid,
               cmap='binary',
               origin='lower',
               extent=[min(xs) - 0.5, max(xs) + 0.5, min(ys) - 0.5, max(ys) + 0.5],
               label='Bresenham')

    theta = np.linspace(0, 2 * np.pi, 720)
    plt.plot(xc + r * np.cos(theta),
             yc + r * np.sin(theta),
             linestyle='--', linewidth=1.5, label='Идеальная окружность')

    plt.xticks(np.arange(min(xs), max(xs) + 1))
    plt.yticks(np.arange(min(ys), max(ys) + 1))
    plt.grid(which='both', linestyle='-', linewidth=0.4)
    plt.gca().set_aspect('equal')
    plt.legend()
    plt.title(f'Circle center=({xc}, {yc}), r={r}')
    plt.xlabel('x'), plt.ylabel('y')
    plt.show()



if __name__ == '__main__':
    print("Введите 1, если отрезок, 2 — если окружность")
    choice = int(input())
    if choice == 1:
        x0 = int(input("x0: "))
        y0 = int(input("y0: "))
        x1 = int(input("x1: "))
        y1 = int(input("y1: "))
        plot_bresenham(x0, y0, x1, y1)
    else:
        xc = int(input("xc: "))
        yc = int(input("yc: "))
        r = int(input("r: "))
        plot_bresenham_circle(xc, yc, r)
