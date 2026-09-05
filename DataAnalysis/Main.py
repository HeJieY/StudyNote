import pandas as pd
import bs4
import Spider

def main():
    print("running")
    spider = Spider.Spider("https://we.51job.com/pc/search")
    df = spider.start(keyword="c++", job_area="000000",
                      max_pages=2, max_jobs=40, click_detail=False)
    output = spider.driver.current_url
    print("fetched:", len(df), "rows")
    print(df[["jobName", "jobAreaString", "provideSalaryString",
              "companyName"]].head())
    return


main()

