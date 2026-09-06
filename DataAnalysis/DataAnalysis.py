import os
import re

import pandas as pd

BASE_DIR = os.path.dirname(os.path.abspath(__file__))


class SkillKeywords:
    """技能关键词集中配置。
    每个关键词用普通字符串即可；若以 '\\b' 开头则当作正则处理（用于单词边界，避免
    像 'go' 误匹配 google、'ai' 误匹配 email 这类情况），其余自动 re.escape。
    """

    LANGUAGE = ["c++", "cpp", "c语言", "c/c++", "java", "\\bgo\\b", "python",
                "rust", "c#", "\\bvc\\b", "\\.net", "shell"]
    DATA_STRUCTURE = ["数据结构", "算法", "哈希", "链表", "动态规划", "排序", "查找"]
    OS = ["linux", "unix", "ubuntu", "centos", "内核", "操作系统", "系统编程", "shell"]
    BUILD = ["cmake", "make", "qmake", "makefile", "gcc", "g++", "clang",
             "交叉编译", "编译器"]
    TOOLS = ["git", "\\bvs\\b", "svn", "vscode", "gdb", "调试", "开发工具"]
    GUI = ["qt", "qt5", "qt6", "mfc", "wxwidgets", "wpf", "上位机",
           "界面开发", "人机交互"]
    EMBEDDED = ["嵌入式", "stm32", "单片机", "\\barm\\b", "\\bmcu\\b",
                "驱动开发", "驱动", "固件", "fpga", "rtos", "freertos",
                "rt-thread", "物联网"]
    NETWORK = ["网络编程", "\\btcp\\b", "\\budp\\b", "\\bhttp\\b", "\\bhttps\\b",
               "socket", "\\bmqtt\\b", "modbus", "串口", "\\bgrpc\\b",
               "protobuf", "websocket", "网络协议"]
    DATABASE = ["数据库", "mysql", "sqlite", "oracle", "sqlserver", "redis",
                "mongodb", "postgresql", "elasticsearch", "数据库优化"]
    CONCURRENCY = ["多线程", "线程池", "多进程", "并发", "协程", "异步",
                   "进程", "线程", "锁", "信号量"]
    BACKEND = ["服务器", "服务端", "后端", "后台", "server", "分布式"]
    MEDIA = ["音视频", "ffmpeg", "流媒体", "\\brtsp\\b", "视频编解码",
             "webrtc", "\\brtmp\\b"]
    IMAGE = ["图像处理", "opencv", "计算机视觉", "图像识别", "halcon",
             "点云", "\\b3d\\b", "三维重建", "目标检测"]
    AI = ["深度学习", "机器学习", "pytorch", "tensorflow", "神经网络",
          "大模型", "\\bllm\\b", "\\bai\\b", "人工智能"]
    CLOUD = ["docker", "kubernetes", "k8s", "容器", "微服务", "云原生",
             "云计算", "消息队列", "负载均衡"]
    ARCH = ["架构设计", "性能优化", "单元测试", "设计模式", "代码规范",
            "重构", "自动化测试"]
    INDUSTRY = ["机器人", "自动驾驶", "智能驾驶", "工控", "军工", "航天",
                "通信", "电力", "汽车", "银行"]

    ALL = {
        "编程语言": LANGUAGE,
        "数据结构/算法": DATA_STRUCTURE,
        "操作系统/Linux": OS,
        "构建/编译": BUILD,
        "开发工具": TOOLS,
        "GUI/桌面": GUI,
        "嵌入式/驱动": EMBEDDED,
        "网络通信": NETWORK,
        "数据库": DATABASE,
        "并发/多线程": CONCURRENCY,
        "服务器/后端": BACKEND,
        "音视频": MEDIA,
        "图像/视觉": IMAGE,
        "AI/机器学习": AI,
        "容器/云": CLOUD,
        "架构/质量": ARCH,
        "行业领域": INDUSTRY,
    }


class WelfareKeywords:
    HOLIDAY = ["带薪年假", "周末双休", "做五休二", "弹性工作", "法定节假日", "不加班"]
    MONEY = ["年终奖金", "绩效奖金", "股票期权", "项目奖金", "提成", "13薪", "14薪", "15薪", "16薪"]
    INSURANCE = ["五险一金", "六险一金", "补充医疗保险", "商业保险", "住房公积金"]
    LIFE = ["定期体检", "员工旅游", "餐饮补贴", "免费班车", "交通补贴", "通讯补贴",
            "节日福利", "节假日福利", "提供住宿", "子女教育", "生日福利", "餐补",
            "零食下午茶", "人才公寓", "租房补贴"]

    ALL = {
        "休假": HOLIDAY,
        "奖金": MONEY,
        "保险": INSURANCE,
        "生活福利": LIFE,
    }


class DataAnalysis:
    def __init__(self, csv_path=None):
        self.csv_path = csv_path or os.path.join(BASE_DIR, "jobs", "summary.csv")
        self.df = pd.read_csv(self.csv_path)
        print(f"loaded {len(self.df)} jobs from {self.csv_path}")

    @staticmethod
    def _safe_text(value):
        return "" if pd.isna(value) else str(value)

    def dataset_summary(self):
        """数据量总览：使用多少数据、覆盖什么。"""
        sal = self.salary_data()
        n = len(self.df)
        welfare_ok = self.df["welfare"].fillna("").str.len().gt(0).sum() if "welfare" in self.df else 0
        return pd.Series({
            "岗位总数": n,
            "有效薪资岗位数": int(sal["avg_month"].notna().sum()),
            "含职位描述岗位数": int(self.df["jobDescribe"].notna().sum()),
            "含福利数据岗位数": int(welfare_ok),
            "公司数": int(self.df["companyName"].nunique()),
            "城市数": int(self.df["jobAreaString"].fillna("未知")
                         .str.split("·").str[0].nunique()),
            "学历要求种类": self.df["degreeString"].nunique(),
            "覆盖行业数": self.df["industryType1Str"].nunique(),
        })

    def describe(self):
        return pd.concat([
            self.df["jobDescribe"].apply(len).describe().rename("职位描述长度(字符)"),
            self.df["jobDescribe"].fillna("").str.lower().str.len().rename("描述小写长度"),
        ], axis=1)

    @staticmethod
    def _compile(keyword):
        if keyword.startswith("\\b"):
            return re.compile(keyword, re.IGNORECASE)
        return re.compile(re.escape(keyword), re.IGNORECASE)

    def skill_frequency(self, keyword_cat="ALL"):
        cats = keyword_cat if isinstance(keyword_cat, dict) else SkillKeywords.ALL
        rows = []
        for cat, words in cats.items():
            for w in words:
                pattern = self._compile(w)
                texts = self.df["jobDescribe"].fillna("") + " " + self.df["jobTags"].fillna("")
                matched = texts.str.contains(pattern, regex=True)
                occurrence = texts.str.findall(pattern).str.len().sum()
                rows.append({
                    "category": cat,
                    "keyword": w,
                    "occurrence": int(occurrence),
                    "job_mentions": int(matched.sum()),
                    "job_rate": round(matched.mean() * 100, 1),
                })
        df = pd.DataFrame(rows)
        df["total_jobs"] = len(self.df)
        return df

    def skill_summary(self):
        freq = self.skill_frequency()
        s = freq.groupby("category").agg(
            keyword_count=("keyword", "count"),
            total_occurrence=("occurrence", "sum"),
            max_rate=("job_rate", "max"),
            top_keyword=("keyword", lambda x: freq.loc[x.index, "keyword"]),
        )
        return s

    @staticmethod
    def _parse_salary_unit(amount, unit):
        if unit == "千":
            return float(amount) * 1000
        if unit == "万":
            return float(amount) * 10000
        return float(amount) * 10000

    @staticmethod
    def parse_salary(value):
        s = DataAnalysis._safe_text(value).strip()
        result = {"raw": s, "min_month": None, "max_month": None,
                  "avg_month": None, "annual_months": None, "annual_avg": None}
        if not s or s in ("面议", "薪资面议"):
            return result

        m = re.search(r"([\d.]+)\s*千\s*-\s*([\d.]+)\s*万", s)
        if m:
            result["min_month"] = float(m.group(1)) * 1000
            result["max_month"] = float(m.group(2)) * 10000
        else:
            m = re.search(r"([\d.]+)-([\d.]+)\s*万", s)
            if m:
                result["min_month"] = float(m.group(1)) * 10000
                result["max_month"] = float(m.group(2)) * 10000
            else:
                m = re.search(r"([\d.]+)\s*(千|万)", s)
                if m:
                    v = DataAnalysis._parse_salary_unit(*m.groups())
                    result["min_month"] = result["max_month"] = v

        if result["min_month"] is not None:
            result["avg_month"] = (result["min_month"] + result["max_month"]) / 2

        m = re.search(r"(\d+)薪", s)
        if m:
            result["annual_months"] = int(m.group(1))

        m = re.search(r"([\d.]+)\s*万/(年|月)", s)
        if m:
            val = float(m.group(1)) * 10000
            if m.group(2) == "年":
                result["min_month"] = result["max_month"] = result["avg_month"] = val / 12
            else:
                result["min_month"] = result["max_month"] = result["avg_month"] = val

        if result["avg_month"] is not None:
            result["annual_avg"] = result["avg_month"] * (result["annual_months"] or 12)
        return result

    def salary_data(self):
        parsed = self.df["provideSalaryString"].apply(self.parse_salary)
        cols = pd.DataFrame(parsed.tolist())
        return pd.concat([self.df[["jobId", "jobName", "companyName"]], cols], axis=1)

    def welfare_frequency(self, welfare_cat="ALL"):
        cats = welfare_cat if isinstance(welfare_cat, dict) else WelfareKeywords.ALL
        rows = []
        for cat, words in cats.items():
            for w in words:
                matched = self.df["welfare"].fillna("").str.contains(re.escape(w), regex=True)
                rows.append({
                    "category": cat,
                    "keyword": w,
                    "job_mentions": int(matched.sum()),
                    "job_rate": round(matched.mean() * 100, 1),
                })
        return pd.DataFrame(rows)

    def welfare_summary(self):
        freq = self.welfare_frequency()
        return freq.groupby("category").agg(
            keyword_count=("keyword", "count"),
            total_jobs=("job_mentions", "sum"),
        )

    def area_distribution(self):
        cities = self.df["jobAreaString"].fillna("未知").str.split("·").str[0]
        return cities.value_counts()

    def company_type_distribution(self):
        return self.df["companyTypeString"].fillna("未知").value_counts()

    def company_size_distribution(self):
        return self.df["companySizeString"].fillna("未知").value_counts()

    def degree_distribution(self):
        return self.df["degreeString"].fillna("未知").value_counts()

    def workyear_distribution(self):
        return self.df["workYearString"].fillna("未知").value_counts()

    def keyword_trend(self, keywords, column="jobDescribe"):
        texts = self.df[column].fillna("")
        rows = []
        for kw in keywords:
            pattern = self._compile(kw)
            rows.append({
                "keyword": kw,
                "occurrence": int(texts.str.findall(pattern).str.len().sum()),
                "job_mentions": int(texts.str.contains(pattern, regex=True).sum()),
            })
        return pd.DataFrame(rows)

    # ------------------------------------------------------------------
    # 学生视角分析
    # ------------------------------------------------------------------

    def _col(self, name, default=False):
        if name in self.df.columns:
            return self.df[name].fillna(default).astype(bool)
        return pd.Series(default, index=self.df.index)

    def student_friendly_mask(self):
        """应届/在校/实习/校招可投岗位（经验要求不高）。"""
        work_year = self.df["workYearString"].fillna("")
        intern = self.df["termStr"].fillna("").str.contains("实习", na=False) | self._col("isIntern")
        xyx = self._col("isFromXyx")
        low_exp = work_year.str.contains("应届|在校|实习|不限经验|0-1年|1年以内|无经验", na=False)
        return low_exp | intern | xyx

    def intern_mask(self):
        """实习岗位。"""
        return self.df["termStr"].fillna("").str.contains("实习", na=False) | self._col("isIntern")

    def student_summary(self):
        n = len(self.df)
        f = self.student_friendly_mask()
        intern = self.intern_mask()
        bachelor_up = self.df["degreeString"].isin(["本科", "硕士", "博士"])
        rows = [
            {"分类": "应届/校招/实习可投", "岗位数": int(f.sum()),
             "占比%": round(f.mean() * 100, 1)},
            {"分类": "实习岗位", "岗位数": int(intern.sum()),
             "占比%": round(intern.mean() * 100, 1)},
            {"分类": "本科及以上可投", "岗位数": int(bachelor_up.sum()),
             "占比%": round(bachelor_up.mean() * 100, 1)},
            {"分类": "大专即可投", "岗位数": int(self.df["degreeString"].isin(["大专"]).sum()),
             "占比%": round(self.df["degreeString"].isin(["大专"]).mean() * 100, 1)},
        ]
        return pd.DataFrame(rows)

    def student_friendly_jobs(self, top=15):
        """应届友好岗位列表（按薪资排序）。"""
        df = self.df[self.student_friendly_mask()].copy()
        sal = self.salary_data()[["avg_month", "annual_avg"]]
        out = df[["jobName", "companyName", "jobAreaString",
                  "workYearString", "degreeString", "provideSalaryString"]].copy()
        out["avg_month"] = sal.loc[df.index, "avg_month"].values
        out["annual_avg"] = sal.loc[df.index, "annual_avg"].values
        return out.sort_values("avg_month", ascending=False).head(top)

    def skill_salary_diff(self, keywords=None):
        """热门技能对薪资的影响：掌握该技能的岗位均薪 vs 未掌握岗位均薪。"""
        sal = self.salary_data()["avg_month"]
        texts = self.df["jobDescribe"].fillna("") + " " + self.df["jobTags"].fillna("")
        words = keywords or ["linux", "qt", "算法", "多线程", "数据结构", "网络编程",
                             "opencv", "嵌入式", "docker", "ffmpeg", "大模型", "机器人"]
        rows = []
        for w in words:
            pattern = self._compile(w)
            hit = texts.str.contains(pattern, regex=True)
            with_mean = sal[hit.fillna(False)].mean()
            without_mean = sal[~hit.fillna(False)].mean()
            rows.append({
                "skill": w,
                "jobs_with": int(hit.sum()),
                "avg_with": with_mean,
                "avg_without": without_mean,
                "premium": with_mean - without_mean,
            })
        return pd.DataFrame(rows)

    def city_salary_summary(self):
        """一级城市维度：岗位数与平均月薪。"""
        city = self.df["jobAreaString"].fillna("未知").str.split("·").str[0]
        sal = self.salary_data()["avg_month"]
        g = pd.DataFrame({"city": city, "avg_month": sal}).groupby("city")
        out = pd.DataFrame({"jobs": g.size(), "avg_month": g["avg_month"].mean()})
        out = out.sort_values("avg_month", ascending=False)
        return out

    def top_employers(self, top=10):
        """招聘主力企业：岗位数最多 + 均薪。"""
        g = self.df.groupby(["companyName", "companyTypeString", "companySizeString"])
        return g.size().rename("jobs").sort_values(ascending=False).head(top)