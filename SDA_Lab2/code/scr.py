"""
ЛР2, варіант 6: S = (1+x)^n = sum C(n,i) x^i, x = 0.2.
Будує 3 графіки похибки |exact - theoretical| залежно від n:
  1) error_cykly.png      - вкладені цикли
  2) error_dynamic.png    - динамічне програмування
  3) error_combined.png   - обидва способи разом

Похибка рахується так само, як у ваших C-програмах (double, той самий порядок операцій),
тому значення збігаються з таблицею у звіті.

Запуск:  python plot_error.py
Потрібно: matplotlib  (pip install matplotlib)
"""
import matplotlib
matplotlib.use("Agg")                      # без вікна, лише збереження у файл
import matplotlib.pyplot as plt

X = 0.2

# --- Які n малювати ---------------------------------------------------------
NS = [1, 2, 3, 10, 20, 30, 50, 100]        # як у таблиці звіту
# NS = list(range(1, 101))                  # <- розкоментуйте для густішого графіка

# --- Вісь Y: 'symlog' показує і нулі, і значення 1e-16 ... 1e-7 на одному графіку
Y_SCALE = "symlog"                         # 'symlog' | 'log' (у 'log' нульові точки зникають) | 'linear'
LINTHRESH = 1e-16                          # нижче цього порогу шкала лінійна (там лежить 0)


def exact(n):
    """«Точне» значення, як у програмах: (1+x) множиться n разів."""
    v = 1.0
    for _ in range(n):
        v *= (1.0 + X)
    return v


def nested(n):
    """Спосіб 1: вкладені цикли (доданок кожного разу рахується з нуля)."""
    res = 0.0
    for i in range(0, n + 1):
        term = 1.0
        for j in range(1, i + 1):
            term *= (n - j + 1) / j * X
        res += term
    return res


def dynamic(n):
    """Спосіб 2: один цикл, динамічне програмування (доданок з попереднього)."""
    cur = 1.0
    res = cur
    for i in range(1, n + 1):
        cur *= (n - i + 1) / i * X
        res += cur
    return res


err_nested = [abs(exact(n) - nested(n)) for n in NS]
err_dynamic = [abs(exact(n) - dynamic(n)) for n in NS]

print(f"{'n':>5} {'Цикли':>14} {'Динамічний':>14}")
for n, a, b in zip(NS, err_nested, err_dynamic):
    print(f"{n:>5} {a:>14.5E} {b:>14.5E}")
print("Похибки збігаються:", err_nested == err_dynamic)

# --- Оформлення (як на графіках кількості операцій) --------------------------
ORANGE, GREEN, BLUE = "#F4742B", "#3BA935", "#1F5F8B"
plt.rcParams.update({"font.family": "DejaVu Sans", "font.size": 10})


def prepare_axes(ax, title):
    ax.set_title(title, fontsize=14, fontweight="bold")
    ax.set_xlabel("n")
    ax.set_ylabel("Похибка |exact − theoretical|")
    ax.set_xlim(0, 100)
    ax.set_xticks(range(0, 101, 10))               # фіксована ціна поділки по X
    if Y_SCALE == "symlog":
        ax.set_yscale("symlog", linthresh=LINTHRESH, linscale=0.6)
        ax.set_ylim(0, 1e-6)
        ax.set_yticks([0] + [10.0 ** k for k in range(-16, -5)])
    elif Y_SCALE == "log":
        ax.set_yscale("log")
    ax.grid(True, which="major", color="#BBBBBB", linewidth=0.7)
    ax.set_axisbelow(True)


def save(fig, name):
    fig.tight_layout()
    fig.savefig(name, dpi=200)
    plt.close(fig)
    print("збережено", name)


# 1) Цикли
fig, ax = plt.subplots(figsize=(7, 4))
ax.plot(NS, err_nested, "-o", color=ORANGE, linewidth=1.6, markersize=6, clip_on=False)
prepare_axes(ax, "Цикли")
save(fig, "error_cykly.png")

# 2) Динамічний
fig, ax = plt.subplots(figsize=(7, 4))
ax.plot(NS, err_dynamic, "-o", color=GREEN, linewidth=1.6, markersize=6, clip_on=False)
prepare_axes(ax, "Динамічний")
save(fig, "error_dynamic.png")

# 3) Обидва разом. Похибки збігаються, тому: тонка суцільна синя лінія з порожніми
#    кільцями-маркерами + товста пунктирна помаранчева з точками всередині — видно обидві.
fig, ax = plt.subplots(figsize=(9, 4.6))
ax.plot(NS, err_nested, "-o", color=BLUE, linewidth=1.2, markersize=11,
        markerfacecolor="none", markeredgewidth=1.6, label="Цикли", zorder=3, clip_on=False)
ax.plot(NS, err_dynamic, linestyle=(0, (4, 3)), marker="o", color=ORANGE, linewidth=3,
        markersize=5, label="Динамічний", zorder=2, clip_on=False)
prepare_axes(ax, "Похибка обчислень")
ax.legend(loc="upper left", frameon=True)
save(fig, "error_combined.png")