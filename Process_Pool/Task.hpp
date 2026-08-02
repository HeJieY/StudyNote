#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <time.h>
#include<stdlib.h>
#include <wait.h>
#define BUFFER_SIZE 16
#define N 10

typedef void (*task_t)();
std::vector<task_t> tasks;

void task1()
{
    printf("这是任务1!\n");
}

void task2()
{
    printf("这是任务2!\n");
}
void task3()
{
    printf("这是任务3!\n");
}
void task4()
{
    printf("这是任务4!\n");
}
void task5()
{
    printf("这是任务5!\n");
}
void loadTask(std::vector<task_t>*tasks)
{
    printf("任务加载了\n");
    tasks->push_back(task1);
    tasks->push_back(task2);
    tasks->push_back(task3);
    tasks->push_back(task4);
    tasks->push_back(task5);
}