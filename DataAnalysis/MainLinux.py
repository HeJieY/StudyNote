"""嵌入式Linux主题完整流程：抓取 -> 分析 -> 独立图表与报告。

所有产物独立命名，不覆盖 C++ 主题：
  数据:   jobs_linux/summary.csv
  图表:   figures_linux/*.png
  报告:   analysis_report_linux.txt
"""
import os
import sys

sys.stdout.reconfigure(encoding="utf-8")

import SpiderLinux
import DataAnalysis
import Visualization

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
JOBS_CSV = os.path.join(BASE_DIR, SpiderLinux.OUT_DIR, "summary.csv")
FIGURES_DIR = os.path.join(BASE_DIR, "figures_linux")
REPORT_PATH = os.path.join(BASE_DIR, "analysis_report_linux.txt")
SUBJECT = "嵌入式Linux岗位"


EMBEDDED_KEYWORDS = {
    "芯片/平台": ["嵌入式", "\\barm\\b", "\\bmcu\\b", "单片机", "stm32",
                 "imx", "瑞芯微", "\\bdsp\\b", "fpga", "risc-v"],
    "操作系统/Linux": ["linux", "内核", "ubuntu", "yocto", "buildroot",
                       "busybox", "系统编程", "系统移植"],
    "驱动/底层": ["驱动开发", "驱动", "设备树", "uboot", "bootloader",
                 "交叉编译", "外设", "gpio", "中断"],
    "网络通信": ["\\btcp\\b", "\\budp\\b", "\\bhttp\\b", "\\bhttps\\b",
               "socket", "以太网", "串口", "\\bcan\\b", "modbus", "mqtt",
               "\\bi2c\\b", "\\bspi\\b", "wifi", "蓝牙", "4g", "nb-iot"],
    "GUI/界面": ["qt", "lvgl", "minigui", "emwin", "界面开发", "触摸屏"],
    "实时/并发": ["rtos", "freertos", "rt-thread", "ucos", "多线程",
                 "线程池", "实时性", "调度", "时间片"],
    "构建/工具": ["cmake", "makefile", "\\bmake\\b", "gcc", "g++", "\\bgdb\\b",
                 "\\bgit\\b", "\\bci\\b", "自动化测试"],
    "音视频/传感": ["ffmpeg", "gstreamer", "rtsp", "音视频", "摄像头",
                   "视频采集", "雷达", "传感器"],
    "AI/智能": ["深度学习", "机器学习", "\\bai\\b", "大模型", "onnx",
               "目标检测", "智能家居", "机器视觉"],
    "安全/可靠性": ["加密", "\\btls\\b", "安全启动", "高性能", "可靠",
                  "稳定", "低功耗"],
}


def run_flow(with_spider=False):
    if with_spider or not os.path.exists(JOBS_CSV):
        SpiderLinux.run_spider()

    analysis = DataAnalysis.DataAnalysis(csv_path=JOBS_CSV)
    Visualization.set_output_dir(FIGURES_DIR)
    Visualization.set_subject(SUBJECT)
    Visualization.render_all(analysis, keyword_cat=EMBEDDED_KEYWORDS)
    return analysis


def _route(analysis):
    sf = analysis.skill_frequency(EMBEDDED_KEYWORDS)
    cat = sf.groupby("category")["occurrence"].sum().sort_values(ascending=False)
    lines = []
    for i, (c, v) in enumerate(cat.head(10).items(), 1):
        top = sf[sf["category"] == c].sort_values(
            "job_mentions", ascending=False).iloc[0]
        lines.append(f"  {i}. {c}（{v} 次） 代表技能: {top['keyword']} "
                     f"{top['job_rate']}%岗位提及")
    return "\n".join(lines)


def _salary_block(analysis):
    sal = analysis.salary_data()
    s = sal["avg_month"].dropna() / 10000
    out = [f"  可解析薪资岗位: {len(s)} / {len(analysis.df)}"]
    if not s.empty:
        out.append(f"  月薪中位数: {s.median():.2f} 万   均值: {s.mean():.2f} 万"
                   f"  最低: {s.min():.2f} 万  最高: {s.max():.2f} 万")
    months = sal["annual_months"].dropna()
    if not months.empty:
        mc = months.value_counts().sort_index()
        out.append("  薪酬月数(X薪): " + "  ".join(
            f"{k}薪×{v}岗" for k, v in mc.items()))
    top_salary = analysis.skill_salary_diff(
        ["linux内核", "驱动", "\\barm\\b", "rtos", "qt", "多线程",
         "\\bai\\b", "大模型", "ffmpeg", "\\bcan\\b"])
    out.append("  技能薪资溢价(万元/月):")
    show = top_salary.sort_values("premium", ascending=False)
    show = show.assign(avg_with_wan=show["avg_with"] / 10000,
                       avg_without_wan=show["avg_without"] / 10000,
                       premium_wan=show["premium"] / 10000)
    for _, r in show.iterrows():
        out.append(f"    {r['skill']:<10} 涉及{r['jobs_with']}岗  均薪"
                   f"{r['avg_with_wan']:.2f}万  溢价{r['premium_wan']:+.2f}万")
    return "\n".join(out)


def _welfare_block(analysis):
    wf = analysis.welfare_frequency()
    top = wf.sort_values("job_mentions", ascending=False).head(10)
    lines = ["  福利Top10(岗位数/占比):"]
    for _, r in top.iterrows():
        lines.append(f"    {r['keyword']:<8} {r['job_mentions']} ({r['job_rate']:.1f}%)")
    cat = wf.groupby("category")["job_mentions"].sum().sort_values(ascending=False)
    lines.append("  福利大类: " + "  ".join(f"{c}×{v}" for c, v in cat.items()))
    return "\n".join(lines)


def _student_block(analysis):
    lines = ["  岗位可投性:"]
    for _, r in analysis.student_summary().iterrows():
        lines.append(f"    {r['分类']}: {r['岗位数']} ({r['占比%']}%)")
    easy = analysis.student_friendly_jobs(top=5)
    if not easy.empty:
        lines.append("  应届/校招/实习可投岗位示例:")
        for _, r in easy.iterrows():
            lines.append(f"    {r['jobName']} | {r['companyName']} | "
                         f"{r['jobAreaString']} | {r['provideSalaryString']}")
    city = analysis.city_salary_summary()
    city = city[city["jobs"] >= 3].head(8)
    city_txt = "  ".join(f"{k}{v:.2f}万" for k, v
                         in (city["avg_month"] / 10000).items())
    lines.append(f"  主招城市(均薪≥0.0万): {city_txt}")
    return "\n".join(lines)


def build_report(analysis):
    summary = analysis.dataset_summary()
    base_dist = {
        "学历": analysis.degree_distribution(),
        "经验要求": analysis.workyear_distribution(),
        "公司性质": analysis.company_type_distribution(),
        "公司规模": analysis.company_size_distribution(),
    }
    dist_txt = "\n".join(f"  【{k}】\n" + "\n".join(
        f"    {name}: {v}" for name, v in s.items()) for k, s in base_dist.items())
    area = analysis.area_distribution()

    return f"""
================ 数据规模（本次使用了多少数据） ================
{summary.to_string()}

================ 技术栈计数侧重（嵌入式Linux方向） ================
{_route(analysis)}

================ 技术栈高频词明细 Top15 ================
{analysis.skill_frequency(EMBEDDED_KEYWORDS).sort_values('occurrence', ascending=False).head(15)[['category','keyword','occurrence','job_rate']].to_string(index=False)}

================ 薪资情况 ================
{_salary_block(analysis)}

================ 休假/福利 ================
{_welfare_block(analysis)}

================ 地区分布（一级城市Top15） ================
{area.head(15).to_string()}

================ 学历/经验/公司维度 ================
{dist_txt}

================ 学生视角 ================
{_student_block(analysis)}
"""


def main():
    analysis = run_flow(with_spider=False)
    report = build_report(analysis)
    with open(REPORT_PATH, "w", encoding="utf-8") as f:
        f.write(report)
    print("\n" + report)
    print(f"report saved: {REPORT_PATH}")


if __name__ == "__main__":
    main()