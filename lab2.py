import numpy as np
import matplotlib.pyplot as plt

# точное решение
E5 = np.exp(5)
C_A = 2/15
C_B = 2/25
C_C = 4/125
C_D = -92/375
C_E = 121/625

def x_exact(t):
    # точное решение
    # x(t) =  2/15 t^3 + 2/25 t^2 + 4/125 t − 92/375 + 121/625 (e^{5t} − e^{5})
    t = np.asarray(t, dtype=float)
    return (C_A*t**3 +
            C_B*t**2 +
            C_C*t +
            C_D +
            C_E*(np.exp(5*t) - E5))

# метод прогонки
def solve_fd_variant17(N, a=0.0, b=1.0):
    h = (b - a) / N                          # шаг сетки
    t = np.linspace(a, b, N + 1)             # узлы t₀…t_N

    p, q = -5.0, 0.0
    b_i = -2.0 - h*p - h**2*q
    c_i =  1.0 + h*p
    g = -2.0 * t[1:-1]**2 * h**2

    l1, m1 = 1.0, -h
    l2, m2 = 0.0,  0.0
    delta = np.empty(N)
    eta   = np.empty(N)

    delta[0] = l1
    eta[0]   = m1

    for i in range(1, N):
        denom   = b_i + delta[i-1]
        delta[i] = -c_i / denom
        eta[i]   = (g[i-1] - eta[i-1]) / denom

    x = np.empty(N + 1)
    x[N] = (l2 * eta[N-1] + m2) / (1.0 - l2 * delta[N-1])

    for i in range(N-1, -1, -1):
        x[i] = delta[i] * x[i+1] + eta[i]

    return t, x




# метод стрельбы
def solve_shooting_variant17(N, a=0.0, b=1.0):
    t_vals = np.linspace(a, b, N + 1)
    h = (b - a) / N

    p, q = -5.0, 0.0
    b_fd = np.zeros(N + 1)
    c_fd = np.zeros(N + 1)
    g_fd = np.zeros(N + 1)

    b_fd[1:N] = -2.0 - h * p - h**2 * q
    c_fd[1:N] =  1.0 + h * p
    for i in range(1, N):
        g_fd[i] = -2.0 * t_vals[i]**2 * h**2

    u = np.zeros(N + 1)
    v = np.zeros(N + 1)
    w = np.zeros(N + 1)

    u[0], u[1] = 1.0, 0.0
    v[0], v[1] = 1.0, 1.0

    for i in range(1, N):
        u[i+1] = -(u[i-1] + b_fd[i] * u[i]) / c_fd[i]
        v[i+1] = -(v[i-1] + b_fd[i] * v[i]) / c_fd[i]
        w[i+1] =  (g_fd[i] - w[i-1] - b_fd[i] * w[i]) / c_fd[i]

    lambda1, mu1 = 1.0, -h
    lambda2, mu2 = 0.0,  0.0

    alpha1 = 1.0
    beta1  = 1.0 - lambda1
    gamma1 = mu1

    alpha2 = u[N] - lambda2 * u[N-1]
    beta2  = v[N] - lambda2 * v[N-1]
    gamma2 = lambda2 * w[N-1] + mu2 - w[N]

    det = alpha1 * beta2 - alpha2 * beta1
    C1 = (gamma1 * beta2 - gamma2 * beta1) / det
    C2 = (alpha1 * gamma2 - alpha2 * gamma1) / det

    x_sh = C1 * u + C2 * v + w
    return t_vals, x_sh

if __name__ == "__main__":
    N = 500                           
    t_fd, x_fd = solve_fd_variant17(N)
    t_sh, x_sh = solve_shooting_variant17(N)
    x_ex = x_exact(t_fd)


    plt.figure(figsize=(7, 4))

    plt.plot(t_fd, x_ex, color="black", linewidth=2.0, label="Точное решение")  
    plt.plot(t_fd, x_fd, '--' ,color="tab:red", linewidth=2.0, label="Конечные разности") 
    plt.plot(t_sh, x_sh, ':',  color="tab:blue", linewidth=2.0, label="Метод стрельбы")  

    plt.title("сравнение решений")
    plt.xlabel("t")
    plt.ylabel("x(t)")
    plt.grid(True, linestyle=":")
    plt.legend()
    plt.tight_layout()
    plt.show()
