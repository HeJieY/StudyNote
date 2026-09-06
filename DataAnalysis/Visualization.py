import os

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import pandas as pd

plt.rcParams["font.sans-serif"] = ["Microsoft YaHei", "SimHei", "Arial Unicode MS"]
plt.rcParams["axes.unicode_minus"] = False

OUTPUT_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "figures")


def ensure_output_dir():
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    return OUTPUT_DIR


def _save(fig, name):
    path = os.path.join(ensure_output_dir(), name)
    fig.tight_layout()
    fig.savefig(path, dpi=150, bbox_inches="tight")
    plt.close(fig)
    print(f"figure saved: {path}")


def _n_suffix(n):
    return f"（N={n} 条岗位）" if n else ""


def plot_skill_frequency(skill_df, top_n=15, name="skill_frequency.png", n=None):
    df = skill_df.sort_values("occurrence", ascending=False).head(top_n)
    if n is None and "total_jobs" in df:
        n = int(df["total_jobs"].iloc[0])
    fig, ax = plt.subplots(figsize=(10, 7))
    ax.barh(df["keyword"], df["occurrence"], color="steelblue")
    for i, (pos, rate) in enumerate(zip(df["occurrence"], df["job_rate"])):
        ax.text(pos, i, f" [{rate}%]", va="center", fontsize=9)
    ax.set_yticks(range(len(df)))
    ax.set_yticklabels(df["keyword"])
    ax.invert_yaxis()
    ax.set_title(f"C++岗位 专业技能关键词出现次数 Top{top_n}{_n_suffix(n)}")
    ax.set_xlabel("出现次数（括号内为该技能被提及的岗位占比）")
    ax.grid(axis="x", linestyle="--", alpha=0.4)
    _save(fig, name)


def plot_skill_category(skill_df, name="skill_category.png", n=None):
    s = skill_df.groupby("category")["occurrence"].sum().sort_values(ascending=True)
    fig, ax = plt.subplots(figsize=(9, 6))
    colors = plt.cm.viridis(s.values / s.sum())
    ax.barh(s.index, s.values, color=colors)
    ax.set_title(f"技能大类出现次数汇总{_n_suffix(n)}")
    ax.set_xlabel("出现次数")
    ax.grid(axis="x", linestyle="--", alpha=0.4)
    _save(fig, name)


def _box_of_salary(ax, series, title, ylabel):
    valid = series.dropna()
    if valid.empty:
        ax.text(0.5, 0.5, "无数据", ha="center")
        ax.set_title(title)
        return
    ax.boxplot(valid.values / 10000, vert=False)
    ax.set_yticklabels(["月薪(万)"])
    ax.set_title(title)
    ax.set_xlabel(ylabel)
    ax.grid(alpha=0.3)


def plot_salary(salary_df, name="salary.png", n=None):
    n = n or int(salary_df["avg_month"].notna().sum())
    fig, axes = plt.subplots(1, 2, figsize=(12, 5))
    _box_of_salary(axes[0], salary_df["avg_month"],
                   f"C++岗位 平均月薪分布{_n_suffix(n)}", "月薪(万元)")
    _box_of_salary(axes[1], salary_df["annual_avg"],
                   "C++岗位 平均年薪分布", "年薪(万元)")
    _save(fig, name)


def plot_salary_range(salary_df, name="salary_range.png", n=None):
    df = salary_df.dropna(subset=["min_month", "max_month"]).copy()
    df = df.sort_values("avg_month", ascending=False).head(15)
    df["job"] = df["jobName"].str.slice(0, 18) + "·" + df["companyName"].str.slice(0, 10)

    fig, ax = plt.subplots(figsize=(11, 7))
    y = range(len(df))
    ax.barh(y, (df["max_month"] - df["min_month"]) / 10000,
            left=df["min_month"] / 10000, color="skyblue", edgecolor="k")
    ax.set_yticks(list(y))
    ax.set_yticklabels(df["job"])
    ax.invert_yaxis()
    ax.set_title(f"Top15 岗位薪资区间（万元/月，条带长度为区间跨度）{_n_suffix(n)}")
    ax.set_xlabel("月薪(万元)")
    ax.grid(axis="x", linestyle="--", alpha=0.4)
    _save(fig, name)


def plot_salary_months(salary_df, name="salary_months.png", n=None):
    months = salary_df["annual_months"].dropna().value_counts().sort_index()
    if months.empty:
        print("no annual-months data (x薪)")
        return
    n = n or int(months.sum())
    fig, ax = plt.subplots(figsize=(8, 5))
    ax.bar(months.index.astype(str), months.values, color="tomato")
    total = months.sum()
    for i, v in enumerate(months.values):
        ax.text(i, v + 0.1, f"{v} ({v / total * 100:.0f}%)", ha="center")
    ax.set_title(f"薪酬月数分布（N薪 指一年发放N个月薪资，标注单位数中 {total} 个带N薪）{_n_suffix(n)}")
    ax.set_xlabel("薪酬月数")
    ax.set_ylabel("岗位数")
    ax.grid(axis="y", linestyle="--", alpha=0.4)
    _save(fig, name)


def plot_welfare(welfare_df, name="welfare.png", n=None):
    df = welfare_df.sort_values("job_mentions", ascending=False).head(12)
    fig, ax = plt.subplots(figsize=(10, 6))
    ax.bar(df["keyword"], df["job_mentions"], color="mediumseagreen")
    for i, (v, rate) in enumerate(zip(df["job_mentions"], df["job_rate"])):
        ax.text(i, v + 0.1, f"{v} ({rate:.0f}%)", ha="center", fontsize=9)
    ax.set_title(f"福利/休假关键词出现岗位数 Top12{_n_suffix(n)}")
    ax.set_xlabel("福利关键词")
    ax.set_ylabel("提及岗位数（含比例）")
    plt.setp(ax.get_xticklabels(), rotation=35, ha="right")
    ax.grid(axis="y", linestyle="--", alpha=0.4)
    _save(fig, name)


def plot_welfare_category(welfare_df, name="welfare_category.png", n=None):
    s = welfare_df.groupby("category")["job_mentions"].sum().sort_values(ascending=True)
    fig, ax = plt.subplots(figsize=(8, 5))
    ax.barh(s.index, s.values, color="darkseagreen")
    ax.set_title(f"福利/休假 大类提及汇总{_n_suffix(n)}")
    ax.set_xlabel("提及岗位总数")
    ax.grid(axis="x", linestyle="--", alpha=0.4)
    _save(fig, name)


def plot_distribution(series, title, name, kind="barh", n=None, top_n=None):
    series = series.copy()
    merged = None
    if top_n is not None and len(series) > top_n:
        merged = int(series.iloc[top_n:].sum())
        series = series.iloc[:top_n]
        if merged > 0:
            series.loc["其他"] = merged

    fig, ax = plt.subplots(figsize=(10, 7))
    if kind == "pie":
        ax.pie(series.values, labels=series.index, autopct="%1.1f%%",
               startangle=90, counterclock=False)
        ax.set_title(f"{title}{_n_suffix(n)}")
    else:
        ax.barh(series.index, series.values, color="slateblue")
        ax.set_title(f"{title}{_n_suffix(n)}")
        ax.set_xlabel("岗位数")
        ax.grid(axis="x", linestyle="--", alpha=0.4)
        if merged is not None:
            ax.text(series.iloc[-1], len(series) - 1, f"  {series.iloc[-1]} 个城市/{merged}岗",
                    va="center", fontsize=9)
    _save(fig, name)


def plot_student_summary(student_df, name="student_summary.png", n=None):
    fig, ax = plt.subplots(figsize=(8, 5))
    colors = ["royalblue", "tomato", "mediumseagreen", "goldenrod"]
    ax.bar(student_df["分类"], student_df["岗位数"], color=colors)
    for i, (v, r) in enumerate(zip(student_df["岗位数"], student_df["占比%"])):
        ax.text(i, v + 0.3, f"{v} ({r}%)", ha="center")
    ax.set_title(f"学生视角：岗位可投性分布{_n_suffix(n)}")
    ax.set_ylabel("岗位数")
    ax.set_ylim(0, student_df["岗位数"].max() * 1.25)
    ax.grid(axis="y", linestyle="--", alpha=0.4)
    _save(fig, name)


def plot_city_salary(city_df, top=15, name="city_salary.png", n=None):
    df = city_df.sort_values("avg_month", ascending=False).head(top)
    fig, ax = plt.subplots(figsize=(11, 6))
    y = range(len(df))
    ax.barh(y, df["jobs"], color="steelblue", alpha=0.75, label="岗位数")
    ax2 = ax.twiny()
    ax2.plot(df["avg_month"].fillna(0) / 10000, list(y), "r-o", label="平均月薪(万)")
    ax.set_yticks(list(y))
    ax.set_yticklabels(df.index)
    ax.invert_yaxis()
    ax.set_xlabel("岗位数")
    ax2.set_xlabel("平均月薪(万元)")
    ax.set_title(f"城市招聘热度与薪资双重对比 Top{top}{_n_suffix(n)}")
    lines1, labels1 = ax.get_legend_handles_labels()
    lines2, labels2 = ax2.get_legend_handles_labels()
    ax.legend(lines1 + lines2, labels1 + labels2, loc="lower right")
    ax.grid(axis="x", linestyle="--", alpha=0.4)
    _save(fig, name)


def plot_skill_salary_diff(diff_df, name="skill_salary_diff.png", n=None):
    df = diff_df.dropna(subset=["avg_with"]).copy()
    df = df.sort_values("avg_with", ascending=False).head(12)
    fig, ax = plt.subplots(figsize=(10, 6))
    y = range(len(df))
    ax.barh(y, df["avg_with"] / 10000, color="seagreen", label="掌握该技能的岗位均薪")
    ax.barh(y, df["avg_without"] / 10000, color="lightcoral", alpha=0.7,
            label="未掌握该技能的岗位均薪")
    for i, (w, wout) in enumerate(zip(df["avg_with"], df["avg_without"])):
        if pd.notna(w) and pd.notna(wout):
            d = (w - wout) / 10000
            ax.text(max(w, wout) / 10000 + 0.05, i, f"{d:+.2f}万", va="center", fontsize=9)
    ax.set_yticks(list(y))
    ax.set_yticklabels(df["skill"])
    ax.invert_yaxis()
    ax.set_title(f"掌握热门技能对月薪的影响{_n_suffix(n)}")
    ax.set_xlabel("平均月薪(万元)")
    ax.legend(loc="lower right")
    ax.grid(axis="x", linestyle="--", alpha=0.4)
    _save(fig, name)


def render_all(analysis):
    ensure_output_dir()
    n = len(analysis.df)
    skill_df = analysis.skill_frequency()
    welfare_df = analysis.welfare_frequency()
    salary_df = analysis.salary_data()

    plot_skill_frequency(skill_df, n=n)
    plot_skill_category(skill_df, n=n)
    plot_salary(salary_df, n=n)
    plot_salary_range(salary_df, n=n)
    plot_salary_months(salary_df, n=n)
    plot_welfare(welfare_df, n=n)
    plot_welfare_category(welfare_df, n=n)
    plot_distribution(analysis.area_distribution(), "工作地区分布(一级城市)",
                      "area.png", n=n, top_n=15)
    plot_distribution(analysis.company_type_distribution(), "公司性质分布", "company_type.png", n=n)
    plot_distribution(analysis.company_size_distribution(), "公司规模分布", "company_size.png", n=n)
    plot_distribution(analysis.degree_distribution(), "学历要求分布", "degree.png", n=n)
    plot_student_summary(analysis.student_summary(), n=n)
    plot_city_salary(analysis.city_salary_summary(), n=n)
    plot_skill_salary_diff(analysis.skill_salary_diff(), n=n)
    print("all figures done ->", OUTPUT_DIR)