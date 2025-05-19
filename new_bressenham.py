import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle


def bresenham_x(x0, y0, x1, y1):
    add_y = -1 if y0 >= y1 else 1
    add_x = 1 if x0 <= x1 else -1
    to_add = abs(y0 - y1)
    dots = []
    zap = abs(x1 - x0)
    error = zap // 2
    while x0 != x1 or y0 != y1:
        dots.append((x0, y0))
        x0 += add_x
        error += to_add
        if error >= zap:
            y0 += add_y
            error -= zap
    dots.append((x0, y0))
    print(dots)
    return dots


def bresenham_y(x0, y0, x1, y1):
    add_y = -1 if y0 >= y1 else 1
    add_x = 1 if x0 <= x1 else -1
    to_add = abs(x0 - x1)
    dots = []
    zap = abs(y1 - y0)
    error = zap // 2
    while x0 != x1 or y0 != y1:
        dots.append((x0, y0))
        y0 += add_y
        error += to_add
        if error >= zap:
            x0 += add_x
            error -= zap
    dots.append((x0, y0))
    return dots

def bresenham_circle(xc: int, yc: int, r: int):
    pts = []
    x, y = 0, r
    d = 1 - r
    while x <= y:
        pts.extend([
            (xc + x, yc + y), (xc - x, yc + y),
            (xc + x, yc - y), (xc - x, yc - y),
            (xc + y, yc + x), (xc - y, yc + x),
            (xc + y, yc - x), (xc - y, yc - x)
        ])
        if d < 0:
            d += 2 * x + 3
        else:
            d += 2 * (x - y) + 5
            y -= 1
        x += 1
    return pts


def draw_cells(points, ideal=None, title=""):
    xs, ys = map(np.array, zip(*points))
    xmin, xmax = xs.min(), xs.max()
    ymin, ymax = ys.min(), ys.max()

    fig, ax = plt.subplots(figsize=(8, 8), facecolor='black')
    ax.set_facecolor('black')

    for x, y in points:
        ax.add_patch(Rectangle((x - 0.5, y - 0.5), 1, 1,
                               facecolor='white', edgecolor='none', alpha=0.9))

    if ideal is not None:
        ax.plot(*ideal, '--', lw=1.4, color='green', alpha=0.4)

    for x in range(xmin, xmax + 2):
        ax.plot([x - 0.5, x - 0.5], [ymin - 0.5, ymax + 0.5],
                color='white', lw=2)
    for y in range(ymin, ymax + 2):
        ax.plot([xmin - 0.5, xmax + 0.5], [y - 0.5, y - 0.5],
                color='white', lw=2)

    ax.set_xlim(xmin - 0.5, xmax + 0.5)
    ax.set_ylim(ymin - 0.5, ymax + 0.5)
    ax.set_aspect('equal')
    ax.set_xticks([]), ax.set_yticks([])
    ax.set_title(title, color='white')
    plt.tight_layout()
    plt.show()


def main():
    choice = int(input("1 — отрезок, 2 — окружность: "))

    if choice == 1:
        x0 = int(input("x0: "))
        y0 = int(input("y0: "))
        x1 = int(input("x1: "))
        y1 = int(input("y1: "))
        if (abs(x0-x1)>=abs(y0-y1)):
            pts = bresenham_x(x0, y0, x1, y1)
        else:
            pts = bresenham_y(x0,y0,x1,y1)
        ideal = ([x0, x1], [y0, y1])
        draw_cells(pts, ideal, title=f'Отрезок ({x0},{y0})→({x1},{y1})')

    else:
        xc = int(input("xc (центр X): "))
        yc = int(input("yc (центр Y): "))
        r = int(input("r (радиус): "))

        pts = bresenham_circle(xc, yc, r)
        t = np.linspace(0, 2 * np.pi, 720)
        ideal = (xc + r * np.cos(t), yc + r * np.sin(t))
        draw_cells(pts, ideal, title=f'Окружность (центр=({xc},{yc}), r={r})')


if __name__ == "__main__":
    main()
