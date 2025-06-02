import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import defaultdict
from dataclasses import dataclass
from typing import List, Tuple


# -----------------------------------------------------------
# 1. Брезенхэм-отрезок (как в вашем примере)
# -----------------------------------------------------------
def bresenham_line(x0, y0, x1, y1):
    # Переносим начало в (0,0)
    x_b, y_b = x1 - x0, y1 - y0

    # Симметрии
    flip_y = flip_x = flip_diag = False
    if y_b < 0:
        flip_y, y_b = True, -y_b
    if x_b < 0:
        flip_x, x_b = True, -x_b
    if y_b > x_b:                       # обмен диагонали
        flip_diag, x_b, y_b = True, y_b, x_b

    # Основной цикл
    points = []
    dx, dy = x_b, y_b
    F, x, y = 2 * dy - dx, 0, 0
    while x < x_b:
        points.append((x, y))
        if F >= 0:
            y += 1
            F -= 2 * dx
        x += 1
        F += 2 * dy
    points.append((x_b, y_b))

    # Обратные преобразования
    res = []
    for px, py in points:
        if flip_diag:
            px, py = py, px
        if flip_x:
            px = -px
        if flip_y:
            py = -py
        res.append((px + x0, py + y0))
    return res


# -----------------------------------------------------------
# 2. Рисуем границу полигона на холсте
# -----------------------------------------------------------
def draw_polygon(polygon_points, canvas, boundary_color=1):
    n = len(polygon_points)
    for i in range(n):
        (x0, y0), (x1, y1) = polygon_points[i], polygon_points[(i + 1) % n]
        for x, y in bresenham_line(x0, y0, x1, y1):
            if 0 <= x < canvas.shape[1] and 0 <= y < canvas.shape[0]:
                canvas[y, x] = boundary_color


# -----------------------------------------------------------
# 3. Active Edge List — вспомогательные структуры
# -----------------------------------------------------------
@dataclass
class Edge:
    y_max: int
    x: float
    dx: float               # 1 / наклон ребра


def build_y_table(vertices: List[Tuple[int, int]]):
    y_table = defaultdict(list)
    n = len(vertices)
    for i in range(n):
        (x1, y1), (x2, y2) = vertices[i], vertices[(i + 1) % n]

        if y1 == y2:                       # горизонталь — не берём
            continue
        if y1 > y2:                        # упорядочиваем
            x1, y1, x2, y2 = x2, y2, x1, y1

        dx = (x2 - x1) / (y2 - y1)         # приращение x на 1 строку
        y_table[y1].append(Edge(y_max=y2, x=x1, dx=dx))
    return y_table


# -----------------------------------------------------------
# 4. AEL-заливка c покадровым сохранением
# -----------------------------------------------------------
def fill_polygon_ael_animated(vertices, canvas,
                              boundary_color=1, fill_color=2):
    max_y, max_x = canvas.shape
    frames = []

    y_table = build_y_table(vertices)
    if not y_table:
        return frames

    y_min = min(y_table.keys())
    y_max_scan = max(edge.y_max for lst in y_table.values() for edge in lst)

    ael: List[Edge] = []
    for y in range(y_min, y_max_scan):
        # добавляем новые рёбра
        ael.extend(y_table.get(y, []))
        # убираем те, что закончились
        ael = [e for e in ael if e.y_max > y]
        # сортируем по текущему x
        ael.sort(key=lambda e: e.x)

        # чёт/нечёт — формируем полосы
        for i in range(0, len(ael), 2):
            x_start = int(round(ael[i].x))
            x_end   = int(round(ael[i + 1].x)) - 1
            for x in range(x_start, x_end + 1):
                if 0 <= x < max_x and 0 <= y < max_y:
                    # «не заполняем» границу, чтобы видеть контур
                    if canvas[y, x] != boundary_color:
                        canvas[y, x] = fill_color
        # сохраняем кадр
        frames.append(np.copy(canvas))

        # двигаем x всех рёбер
        for e in ael:
            e.x += e.dx

    return frames


# -----------------------------------------------------------
# 5. Демонстрация
# -----------------------------------------------------------
if __name__ == "__main__":
    # Размер холста
    width, height = 200, 200
    canvas = np.zeros((height, width), dtype=np.uint8)

    # Вершины многоугольника (пример)
    polygon_points = [
        (40, 40),  # A
        (160, 50),  # B
        (160, 80),  # C  ┐ верхний выступ
        (90, 80),  # D  │
        (90, 120),  # E  ┘
        (140, 120),  # F
        (135, 160),  # G  ← острый "зуб"
        (60, 150),  # H
        (50, 200),  # I  ┐ нижний выступ
        (30, 190),  # J  ┘
    ]

    # 1) граница
    draw_polygon(polygon_points, canvas, boundary_color=1)
    # 2) покадровая AEL-заливка
    frames = fill_polygon_ael_animated(
        polygon_points, canvas, boundary_color=1, fill_color=2
    )

    # 3) визуализация
    fig, ax = plt.subplots(figsize=(6, 6))
    if frames:
        img = ax.imshow(frames[0], cmap='gray', origin='lower')
    else:                       # на случай пустого многоугольника
        img = ax.imshow(canvas, cmap='gray', origin='lower')

    def update(k):
        img.set_array(frames[k])
        return [img]

    anim = animation.FuncAnimation(
        fig, update, frames=len(frames),
        interval=150, repeat=False
    )
    plt.title("Алгоритм заполнения: Active Edge List")
    plt.show()
