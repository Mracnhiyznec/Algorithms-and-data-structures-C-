import matplotlib.pyplot as plt
import numpy as np


# 1. Алгоритм Сазерленд–Коэн
# Константы для определения положения точки относительно прямоугольника
inside = 0  # точка внутри окна
left = 1 # точка слева от окна
right = 2 # точка справа от окна
bottom = 4 # точка снизу от окна
top = 8 # точка сверху от окна


def compute_out_code(x, y, x_min, y_min, x_max, y_max):
    """
    Вычисляет код расположения точки (x, y) относительно прямоугольника,
    заданного координатами (x_min, y_min) и (x_max, y_max).
    """
    code = inside
    if x < x_min:
        code = (code | left)
    elif x > x_max:
        code = (code | right)
    if y < y_min:
        code = (code | bottom)
    elif y > y_max:
        code = (code | top)
    return code


def cohen_sutherland_clip(x0, y0, x1, y1, x_min, y_min, x_max, y_max):
    """
    Отсекает отрезок (x0, y0) - (x1, y1) прямоугольником с координатами
    (x_min, y_min, x_max, y_max) по алгоритму Сазерленда–Коэна.

    Возвращает:
      accept (bool) - True, если отрезок (или его часть) находится внутри окна,
      clipped_line (tuple) - координаты отсечённого отрезка (если accept==True)
    """
    out_code0 = compute_out_code(x0, y0, x_min, y_min, x_max, y_max)
    out_code1 = compute_out_code(x1, y1, x_min, y_min, x_max, y_max)

    accept = False

    while True:
        # если оба кода == 0, то наш отрезок ПОЛНОСТЬЮ ВНУТРИ
        if out_code0 == 0 and out_code1 == 0:
            accept = True
            break
        # если побитовое И кодов не равно нулю, значит отрезок ПОЛНОСТЬЮ СНАРУЖИ
        elif (out_code0 & out_code1) != 0:
            break
        else:
            # иначе какая-то точка снаружи, найдём её
            if out_code0 != 0:
                out_code_out = out_code0
            else:
                out_code_out = out_code1

            # Инициализируем пересечение
            x, y = 0.0, 0.0
            # если пересечение с верхней частью окна
            if out_code_out & top:
                x = x0 + (x1 - x0) * (y_max - y0) / (y1 - y0)
                y = y_max
            # если пересечение с нижней частью окна
            elif out_code_out & bottom:
                x = x0 + (x1 - x0) * (y_min - y0) / (y1 - y0)
                y = y_min
            # если пересечение с правой частью окна
            elif out_code_out & right:
                y = y0 + (y1 - y0) * (x_max - x0) / (x1 - x0)
                x = x_max
            # если пересечение с левой частью окна
            elif out_code_out & left:
                y = y0 + (y1 - y0) * (x_min - x0) / (x1 - x0)
                x = x_min

            # смотрим, с какой частью окна будем работать дальше
            if out_code_out == out_code0:
                x0, y0 = x, y
                out_code0 = compute_out_code(x0, y0, x_min, y_min, x_max, y_max)
            else:
                x1, y1 = x, y
                out_code1 = compute_out_code(x1, y1, x_min, y_min, x_max, y_max)
    if accept:
        return True, (x0, y0, x1, y1)
    else:
        return False, (x0, y0, x1, y1)


# =============================================================================
# 2. Алгоритм Цируса–Бека (Cyrus–Beck) для выпуклого многоугольника с внешней нормалью
# =============================================================================

def cyrus_beck_clip_polygon_external_normal(x0, y0, x1, y1, polygon):
    """
    Отсекает отрезок (x0, y0) - (x1, y1) выпуклым многоугольником,
    используя алгоритм Цируса–Бека с вычислением ВНЕШНЕЙ нормали к ребрам.

    Внешняя нормаль вычисляется как: (edge_dy, -edge_dx).

    Возвращает:
      accept (bool): True, если пересечение существует, иначе False.
      clipped_line (tuple): координаты отсечённого отрезка (xx0, yy0, xx1, yy1),
                           если пересечение найдено.
      entering_points (list): список потенциально входящих точек.
      leaving_points (list): список потенциально покидающих точек.
    """
    finalFlag = True

    dx = x1 - x0
    dy = y1 - y0

    t_min = 0.0
    t_max = 1.0

    entering_points = []
    leaving_points = []

    n = len(polygon)
    for i in range(n):
        xA, yA = polygon[i]
        xB, yB = polygon[(i + 1) % n]

        edge_dx = xB - xA
        edge_dy = yB - yA

        # Вычисляем внешнюю нормаль: (edge_dy, -edge_dx)
        normal_x = edge_dy
        normal_y = -edge_dx

        # Вектор от A к началу отрезка
        w_x = x0 - xA
        w_y = y0 - yA

        # Вычисляем скалярные произведения:
        # dot_d = нормаль · (направление отрезка)
        # dot_w = нормаль · (P0 - A)
        dot_d = normal_x * dx + normal_y * dy
        dot_w = normal_x * w_x + normal_y * w_y

        # Если отрезок параллелен ребру (dot_d близко к 0)
        eps = 1e-6
        if abs(dot_d) < eps:
            # Если отрезок параллелен и находится "за" ребром (dot_w > 0) – отсечение невозможно
            if abs(dot_w) > eps:
                finalFlag = False
            else:
                # Параллелен, но по ту же сторону – ничего не меняем для данного ребра
                continue
        else:
            # Вычисляем параметр t пересечения: dot_w + dot_d * t = 0 => t = -dot_w / dot_d
            t = -dot_w / dot_d

            # Вычисляем точку пересечения
            ix = x0 + t * dx
            iy = y0 + t * dy

            # Интерпретация входящей/покидающей точки зависит от знака dot_d:
            # dot_d < 0: при пересечении отрезок "заходит" в область
            # dot_d > 0: отрезок "выходит" из области
            if dot_d < 0:
                entering_points.append((ix, iy))
            else:
                leaving_points.append((ix, iy))

            # Обновляем параметры отсечения
            if dot_d < 0:
                # Потенциальное вхождение – увеличиваем нижнюю границу t_min
                t_min = max(t_min, t)
            else:
                # Потенциальный выход – уменьшаем верхнюю границу t_max
                t_max = min(t_max, t)

            # Если интервал становится пустым, пересечения нет
            if t_min > t_max:
                finalFlag = False

    # Если пересечение найдено – вычисляем координаты отсечённого отрезка
    xx0 = x0 + t_min * dx
    yy0 = y0 + t_min * dy
    xx1 = x0 + t_max * dx
    yy1 = y0 + t_max * dy

    return finalFlag, (xx0, yy0, xx1, yy1), entering_points, leaving_points


# =============================================================================
# 3. Алгоритм "средней точки" (Midpoint Subdivision) для прямоугольного окна
# =============================================================================

def midpoint_subdivision_clip(x0, y0, x1, y1, x_min, y_min, x_max, y_max, depth=10):
    """
    Отсекает отрезок (x0, y0) - (x1, y1) прямоугольником (x_min, y_min, x_max, y_max)
    методом рекурсивного деления по средней точке.

    Если отрезок целиком внутри окна (trivial accept), возвращает список с одним отрезком.
    Если отрезок целиком снаружи (trivial reject), возвращает пустой список.
    Иначе делит отрезок на две половины и рекурсивно обрабатывает каждую.

    Параметр depth ограничивает глубину рекурсии.
    """
    # проверяем, что длина отрезка меньше размера "пикселя"
    # (допустим, что размер пикселя = 0.001)
    if np.sqrt((x1 - x0) ** 2 + (y1 - y0) ** 2) < 0.001:
        return []

    code0 = compute_out_code(x0, y0, x_min, y_min, x_max, y_max)
    code1 = compute_out_code(x1, y1, x_min, y_min, x_max, y_max)

    # если отрезок ПОЛНОСТЬЮ ВНУТРИ
    if code0 == 0 and code1 == 0:
        return [(x0, y0, x1, y1)]

    # если отрезок ПОЛНОСТЬЮ СНАРУЖИ
    if (code0 & code1) != 0:
        return []

    # если дошли до конца рекурсии – заканчиваем алгоритм
    if depth == 0:
        return []

    # вычисляем среднюю точку
    xm = (x0 + x1) / 2.0
    ym = (y0 + y1) / 2.0

    segs1 = midpoint_subdivision_clip(x0, y0, xm, ym, x_min, y_min, x_max, y_max, depth - 1)
    segs2 = midpoint_subdivision_clip(xm, ym, x1, y1, x_min, y_min, x_max, y_max, depth - 1)

    return segs1 + segs2


# =============================================================================
# Главная функция для визуализации всех алгоритмов
# =============================================================================

def main():
    # Задаём прямоугольное окно для алгоритмов Сазерленда–Коэна и "средней точки"
    rect_x_min, rect_y_min = -5, -3
    rect_x_max, rect_y_max = 5, 4

    # Задаём исходный отрезок
    line_x0, line_y0 = 0, -2
    line_x1, line_y1 = 3, 10
    # line_x0, line_y0 = -2, 3
    # line_x1, line_y1 = 5, -1

    # Задаём выпуклый многоугольник (6 вершин, обход против часовой стрелки)
    polygon = [
        (0, -3),
        (3, -2),
        (4, 1),
        (2, 3),
        (-2, 2),
        (-3, -2),
    ]

    # 1) Отсечение алгоритмом Сазерленда–Коэна (прямоугольник)
    accept_cs, clipped_cs = cohen_sutherland_clip(
        line_x0, line_y0, line_x1, line_y1,
        rect_x_min, rect_y_min, rect_x_max, rect_y_max
    )

    # 2) Отсечение алгоритмом Цируса–Бека с внешней нормалью (многоугольник)
    accept_cb, clipped_cb, entering_points, leaving_points = cyrus_beck_clip_polygon_external_normal(
        line_x0, line_y0, line_x1, line_y1, polygon
    )

    # 3) Отсечение алгоритмом "средней точки" (прямоугольник)
    midpoint_segments = midpoint_subdivision_clip(
        line_x0, line_y0, line_x1, line_y1,
        rect_x_min, rect_y_min, rect_x_max, rect_y_max,
        depth=10
    )

    # Создаём фигуру с 3 подграфиками для визуализации алгоритмов
    fig, axes = plt.subplots(1, 3, figsize=(18, 6))

    # -------------------------------------------------------------------------
    # A) Визуализация алгоритма Сазерленда–Коэна (прямоугольник)
    ax1 = axes[0]
    ax1.set_title("Sutherland–Cohen (прямоугольник)")
    # Рисуем прямоугольник
    rect_x = [rect_x_min, rect_x_max, rect_x_max, rect_x_min, rect_x_min]
    rect_y = [rect_y_min, rect_y_min, rect_y_max, rect_y_max, rect_y_min]
    ax1.plot(rect_x, rect_y, 'b-', label="Прямоугольник")
    # Рисуем исходный отрезок (оранжевый пунктир)
    ax1.plot(
        [line_x0, line_x1],
        [line_y0, line_y1],
        color='orange',
        linestyle='--',
        label="Исходный отрезок"
    )
    # Рисуем отсечённую часть (фиолетовая линия)
    if accept_cs:
        ax1.plot(
            [clipped_cs[0], clipped_cs[2]],
            [clipped_cs[1], clipped_cs[3]],
            color='magenta',
            linewidth=3,
            label="Отсечённая часть"
        )
    ax1.legend()
    ax1.grid(True)
    ax1.set_aspect('equal', adjustable='box')

    # -------------------------------------------------------------------------
    # B) Визуализация алгоритма Цируса–Бека с внешней нормалью (многоугольник)
    ax2 = axes[1]
    ax2.set_title("Cyrus–Beck (внешняя нормаль)")
    # Рисуем многоугольник (замыкаем контур)
    poly_x = [p[0] for p in polygon] + [polygon[0][0]]
    poly_y = [p[1] for p in polygon] + [polygon[0][1]]
    ax2.plot(poly_x, poly_y, 'b-', label="Многоугольник")
    # Рисуем исходный отрезок
    ax2.plot(
        [line_x0, line_x1],
        [line_y0, line_y1],
        color='orange',
        linestyle='--',
        label="Исходный отрезок"
    )
    # Рисуем потенциально входящие точки (зелёные кружки)
    if entering_points:
        ex, ey = zip(*entering_points)
        ax2.scatter(ex, ey, color='green', marker='o', s=60, label="Входящие точки")
    # Рисуем потенциально покидающие точки (красные крестики)
    if leaving_points:
        lx, ly = zip(*leaving_points)
        ax2.scatter(lx, ly, color='red', marker='x', s=60, label="Покидающие точки")
    # Рисуем отсечённую часть отрезка
    if accept_cb:
        ax2.plot(
            [clipped_cb[0], clipped_cb[2]],
            [clipped_cb[1], clipped_cb[3]],
            color='magenta',
            linewidth=3,
            label="Отсечённая часть"
        )
    ax2.legend()
    ax2.grid(True)
    ax2.set_aspect('equal', adjustable='box')

    # -------------------------------------------------------------------------
    # C) Визуализация алгоритма "средней точки" (прямоугольник)
    ax3 = axes[2]
    ax3.set_title("Midpoint Subdivision (прямоугольник)")
    # Рисуем прямоугольник
    ax3.plot(rect_x, rect_y, 'b-', label="Прямоугольник")
    # Рисуем исходный отрезок
    ax3.plot(
        [line_x0, line_x1],
        [line_y0, line_y1],
        color='orange',
        linestyle='--',
        label="Исходный отрезок"
    )
    # Рисуем каждый полученный подотрезок, который оказался внутри
    for seg in midpoint_segments:
        x0m, y0m, x1m, y1m = seg
        ax3.plot([x0m, x1m], [y0m, y1m], color='magenta', linewidth=2)
    ax3.legend()
    ax3.grid(True)
    ax3.set_aspect('equal', adjustable='box')

    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    main()
