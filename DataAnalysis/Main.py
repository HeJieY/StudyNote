import os
import sys

sys.stdout.reconfigure(encoding="utf-8")

import pandas as pd

import Spider
import DataAnalysis
import Visualization

BASE_DIR = os.path.dirname(os.path.abspath(__file__))


def run_spider(keyword="c++", max_pages=50, max_jobs=1000):
    spider = Spider.Spider("https://we.51job.com/pc/search")
    df = spider.start(keyword=keyword, job_area="000000",
                      max_pages=max_pages, max_jobs=max_jobs,
                      click_detail=False)
    return df


def run_analysis(with_spider=False):
    if with_spider:
        run_spider()
    analysis = DataAnalysis.DataAnalysis()
    Visualization.render_all(analysis)
    return analysis


def _lines(analysis):
    summary = analysis.dataset_summary()

    city = analysis.city_salary_summary()
    city = city[city["jobs"] >= 5].head(10)
    city_txt = (city.assign(avg_wan=city["avg_month"] / 10000)
                [["jobs", "avg_wan"]].round(2).to_string())

    return f"""
================ 数据规模（本次使用了多少数据） ================
{summary.to_string()}

================ 学生视角：岗位可投性 ================
{analysis.student_summary().to_string(index=False)}

================ 学生视角：应届友好岗位（按月薪排序） ================
{easy_txt(analysis)}

================ 学生视角：热门技能是否带来薪资溢价 ================
{diff_txt(analysis)}

================ 学生视角：城市招聘热度与薪资 Top10（岗位数>=5） ================
{city_txt}

================ 学生视角：招聘主力企业 Top8 ================
{analysis.top_employers(top=8).to_string()}

================ 学生视角：技能学习重点（建议学习路线） ================
{route_txt(analysis)}
"""


def easy_txt(analysis, top=10):
    easy = analysis.student_friendly_jobs(top=top)
    if easy.empty:
        return "（当前样本中无应届/校招/实习可投岗位）"
    return easy[["jobName", "companyName", "jobAreaString",
                 "provideSalaryString"]].to_string(index=False)


def diff_txt(analysis):
    show = analysis.skill_salary_diff().sort_values("premium", ascending=False)
    show = show.assign(avg_with_wan=show["avg_with"] / 10000,
                       avg_without_wan=show["avg_without"] / 10000,
                       premium_wan=show["premium"] / 10000)
    return show[["skill", "jobs_with", "avg_with_wan",
                 "avg_without_wan", "premium_wan"]].round(2).to_string(index=False)


def route_txt(analysis):
    sf = analysis.skill_frequency()
    cat = sf.groupby("category")["occurrence"].sum().sort_values(ascending=False)
    lines = []
    for i, (c, v) in enumerate(cat.head(8).items(), 1):
        top_key = sf[sf["category"] == c].sort_values(
            "job_mentions", ascending=False).iloc[0]
        lines.append(f"  {i}. {c}（{v} 次） 代表技能: {top_key['keyword']} "
                     f"{top_key['job_rate']}%岗位提及")
    return "\n".join(lines)


def write_report(analysis):
    out = os.path.join(BASE_DIR, "analysis_report.txt")
    with open(out, "w", encoding="utf-8") as f:
        f.write(_lines(analysis))
    print(f"report saved: {out}")
    return out


if __name__ == "__main__":
    analysis = run_analysis(with_spider=False)
    write_report(analysis)