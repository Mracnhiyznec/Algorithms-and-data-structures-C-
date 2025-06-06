import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# 1. Параметры сетки
hx = hy = 0.1                      # шаг сетки
x_min, x_max = -3, 3               # прямоугольник, в который вписан эллипс
y_min, y_max = -4, 4

x = np.arange(x_min, x_max + hx, hx)
y = np.arange(y_min, y_max + hy, hy)
X, Y = np.meshgrid(x, y, indexing='ij')

# 2. Определяем, где эллипс, граница и «внутренность»
ellipse = (X**2)/9 + (Y**2)/16             # выражение эллипса
tol = 0.02                                 # «толщина» полосы для границы
boundary = np.abs(ellipse - 1) <= tol      # узлы ≈ на границе
inside   = ellipse < 1 - tol               # узлы внутри
# Все прочие узлы — вне области; они не участвуют в итерациях,
# но им сразу присвоим граничное значение, чтобы формулы были одинаковыми
u = np.zeros_like(X)
f = np.abs(X) + np.abs(Y)                  # |x|+|y|
u[boundary] = f[boundary]
u[~inside & ~boundary] = f[~inside & ~boundary]

# 3. Метод Якоби
max_it = 5_000
eps    = 1e-4
for k in range(max_it):
    u_new = u.copy()
    # обновляем только внутренние узлы (векторизовано)
    u_new[1:-1, 1:-1][inside[1:-1, 1:-1]] = (
        u[:-2, 1:-1][inside[1:-1, 1:-1]] +
        u[2:, 1:-1][inside[1:-1, 1:-1]] +
        u[1:-1, :-2][inside[1:-1, 1:-1]] +
        u[1:-1, 2:][inside[1:-1, 1:-1]]
    ) * 0.25

    if np.max(np.abs(u_new - u)) < eps:    # сходимость
        print(f'Converged in {k+1} iterations.')
        break
    u = u_new

# 4. График
fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')

# Готовим данные для поверхности (уже в правильной системе координат)
X_plot, Y_plot = np.meshgrid(x, y, indexing='ij')
Z_plot = u  # значения решения

# Рисуем поверхность с цветовой кодировкой
surf = ax.plot_surface(X_plot, Y_plot, Z_plot,
                       cmap='viridis',      # цветовая схема
                       rstride=1,           # шаг по строкам
                       cstride=1,           # шаг по столбцам
                       linewidth=0,
                       antialiased=True,
                       alpha=0.8)

# Настройка осей и меток
ax.set_title('Приближённое решение u(x, y) для варианта 1', pad=15)
ax.set_xlabel('x', labelpad=10)
ax.set_ylabel('y', labelpad=10)
ax.set_zlabel('u', labelpad=10)

# Цветовая шкала
cbar = fig.colorbar(surf, ax=ax, shrink=0.6, aspect=10)
cbar.set_label('Значение u')

# Оптимальный угол обзора
ax.view_init(elev=30, azim=-45)

# Показываем график
plt.tight_layout()
plt.show()
