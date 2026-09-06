import os
import time
import json
import re
import random
from urllib.parse import quote

import pandas as pd
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait

MODEL = 0

FIELDS = [
    "jobId", "jobName", "jobHref", "jobAreaString", "jobAreaCode",
    "workAreaCode", "landmarkString", "provideSalaryString",
    "jobSalaryMin", "jobSalaryMax", "workYear", "workYearString",
    "degreeString", "companyName", "fullCompanyName", "coId",
    "companyTypeString", "companySizeString", "industryType1Str",
    "industryType2Str", "issueDateString", "updateDateTime",
    "applyTimeText", "jobDescribe", "jobTags",
    "jobWelfareCodeDataList",
    "jobType", "hireType", "isIntern", "isFromXyx", "termStr",
    "jobNumString",
]


class Spider:
    def __init__(self, url):
        self.url = url
        self.driver = webdriver.Chrome()
        self.main_handle = None

    def _fetch(self, keyword, job_area, page_num, page_size=20):
        script = """
        async function probe() {
            const ts = Date.now();
            const url = `https://we.51job.com/api/job/search-pc?api_key=51job&timestamp=${ts}`
                + `&keyword=${encodeURIComponent(arguments[0])}&searchType=2&jobArea=${arguments[1]}`
                + `&function=&industry=&jobArea2=&landmark=&metro=&salary=&workYear=&degree=`
                + `&companyType=&companySize=&jobType=&issueDate=&sortType=0`
                + `&pageNum=${arguments[2]}&pageSize=${arguments[3]}&source=1&accountId=`;
            const r = await fetch(url, {credentials: 'include', headers: {'Referer': location.href}});
            return await r.text();
        }
        const cb = arguments[arguments.length - 1];
        probe(arguments[0], arguments[1], arguments[2], arguments[3])
            .then(cb).catch(e => cb(JSON.stringify({error: String(e)})));
        """
        result = self.driver.execute_async_script(script, keyword, job_area, page_num, page_size)
        return json.loads(result)

    def _fetch_all(self, keyword, job_area, max_pages, max_jobs):
        jobs = []
        total = None
        for page in range(1, max_pages + 1):
            if total is not None and len(jobs) >= max_jobs:
                break
            for attempt in range(3):
                data = self._fetch(keyword, job_area, page)
                if data.get("error"):
                    print("api error:", data["error"])
                    break
                body = data.get("resultbody", {})
                job = body.get("job", {})
                items = job.get("items", [])
                if items:
                    break
                print(f"page {page}: empty (attempt {attempt + 1}), retry...")
                time.sleep(random.uniform(3, 6))
            if total is None:
                total = job.get("totalCount")
                print(f"total count: {total}")
            items = job.get("items", [])
            if not items:
                print(f"page {page}: no items, stop")
                break
            jobs.extend(items)
            print(f"page {page}: got {len(items)}, total {len(jobs)}")
            time.sleep(random.uniform(1.5, 3.0))
        return jobs

    def _is_slider_page(self):
        try:
            return "滑动验证" in self.driver.title or bool(
                self.driver.find_elements(By.CSS_SELECTOR, ".nc-container"))
        except Exception:
            return False

    def _wait_slider_pass(self, timeout=120):
        if not self._is_slider_page():
            return True
        print("[!] 检测到滑动验证，请在浏览器中手动完成滑块，爬虫将自动继续...")
        end = time.time() + timeout
        while time.time() < end:
            time.sleep(2)
            if not self._is_slider_page():
                return True
        return False

    def _click_open_detail(self, index):
        handles_before = set(self.driver.window_handles)
        items = self.driver.find_elements(By.CLASS_NAME, "joblist-item")
        if index >= len(items):
            return None, None
        jname = items[index].find_element(By.CSS_SELECTOR, ".jname")
        self.driver.execute_script("arguments[0].scrollIntoView(true);", jname)
        time.sleep(0.5)
        self.driver.execute_script("arguments[0].click();", jname)

        try:
            WebDriverWait(self.driver, 10).until(
                lambda d: len(d.window_handles) > len(handles_before))
        except Exception:
            return None, None
        new_handle = [h for h in self.driver.window_handles
                      if h not in handles_before][0]
        self.driver.switch_to.window(new_handle)
        time.sleep(2)
        if not self._wait_slider_pass():
            raise RuntimeError("slider not passed")

        detail_url = self.driver.current_url
        html = self.driver.page_source
        self.driver.close()
        self.driver.switch_to.window(
            [h for h in self.driver.window_handles if h != new_handle][0])
        return detail_url, html

    def save_jobs(self, df, click_detail=False, out_dir="jobs"):
        os.makedirs(out_dir, exist_ok=True)
        rows = []

        for i in range(min(len(df), 20)):
            if not click_detail:
                break
            row = df.iloc[i]
            try:
                detail_url, html = self._click_open_detail(i)
            except Exception as e:
                print(f"click fail: {row['jobName']} - {e}")
                continue
            if detail_url is None:
                continue
            safe_company = re.sub(r'[\\/:*?"<>|]', "_", str(row["companyName"]))
            safe_title = re.sub(r'[\\/:*?"<>|]', "_", str(row["jobName"]))
            comp_dir = os.path.join(out_dir, f"{safe_company}_{row['coId']}")
            os.makedirs(comp_dir, exist_ok=True)
            fname = f"{row['jobId']}_{safe_title}.html"
            with open(os.path.join(comp_dir, fname), "w", encoding="utf-8") as f:
                f.write(html)
            print(f"saved detail: {row['jobName']} -> {comp_dir}")
            rows.append({"jobId": row["jobId"], "detail_url": detail_url,
                         "html_file": os.path.join(comp_dir, fname)})
            time.sleep(random.uniform(0.5, 1.5))

        comp_df = pd.concat([df, pd.DataFrame(
            {r["jobId"]: r for r in rows}).T] if rows else [df], ignore_index=False)
        for (cname, cid), g in df.groupby(["companyName", "coId"], dropna=False):
            safe_company = re.sub(r'[\\/:*?"<>|]', "_", str(cname))
            comp_dir = os.path.join(out_dir, f"{safe_company}_{cid}")
            os.makedirs(comp_dir, exist_ok=True)
            g.to_csv(os.path.join(comp_dir, "jobs.csv"),
                     index=False, encoding="utf-8-sig")
            print(f"company dir: {comp_dir} ({len(g)} jobs)")

        df.to_csv(os.path.join(out_dir, "summary.csv"),
                  index=False, encoding="utf-8-sig")
        print(f"summary saved: {out_dir}/summary.csv ({len(df)} rows)")
        return df

    def start(self, keyword="c++", job_area="000000", max_pages=5,
              max_jobs=200, click_detail=False):
        url = (f"https://we.51job.com/pc/search?keyword={quote(keyword)}"
               f"&searchType=2&jobArea={job_area}")
        self.driver.get(url)
        time.sleep(8)
        self.main_handle = self.driver.current_window_handle

        jobs = self._fetch_all(keyword, job_area, max_pages, max_jobs)
        df = pd.DataFrame(jobs)
        if df.empty:
            print("no jobs fetched")
            return df

        df = df.drop_duplicates(subset="jobId", keep="first").reset_index(drop=True)
        df = df[[c for c in FIELDS if c in df.columns]]
        df["welfare"] = df["jobWelfareCodeDataList"].apply(
            lambda x: "|".join(w["chineseTitle"] for w in x) if isinstance(x, list) else "")
        return self.save_jobs(df, click_detail=click_detail)