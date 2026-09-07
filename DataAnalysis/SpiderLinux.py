"""抓取 51job『嵌入式Linux』方向岗位列表。

独立于 C++ 分析主题：数据输出到 jobs_linux/ 目录，不覆盖原 jobs/ 数据。
通用抓取逻辑复用于 Spider.Spider，此处仅指定主题、关键词与输出位置。
"""
import sys

from Spider import Spider

KEYWORD = "嵌入式linux"
JOB_AREA = "000000"
OUT_DIR = "jobs_linux"
MAX_PAGES = 50
MAX_JOBS = 1000


def run_spider(keyword=KEYWORD, job_area=JOB_AREA, out_dir=OUT_DIR,
               max_pages=MAX_PAGES, max_jobs=MAX_JOBS):
    spider = Spider("https://we.51job.com/pc/search")
    df = spider.start(keyword=keyword, job_area=job_area,
                      max_pages=max_pages, max_jobs=max_jobs,
                      click_detail=False, out_dir=out_dir)
    return df


if __name__ == "__main__":
    sys.stdout.reconfigure(encoding="utf-8")
    df = run_spider()
    print(f"\n嵌入式Linux抓取完成: {len(df)} 条 -> {OUT_DIR}/summary.csv")