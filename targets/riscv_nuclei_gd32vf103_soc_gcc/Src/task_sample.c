/*
 * Copyright (c) 2013-2019 Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
 */

 #include "task_sample.h"
 #include "los_task.h"
 #include "los_config.h"
 #include <stdio.h>
 #include <string.h>
 
 /* 任务ID */
 UINT32 g_highPriorityTaskID;
 UINT32 g_lowPriorityTaskID;
 
 /* 任务优先级 */
 #define HIGH_TASK_PRIO     4
 #define LOW_TASK_PRIO      5
 
 /* 任务栈大小 */
 #define TASK_STACK_SIZE    0x1000
 
 /* 延时时间 */
 #define DELAY_TICKS        20
 
 /* 高优先级任务函数 */
 VOID HighPriorityTaskEntry(VOID)
 {
     printf("High priority task started, delaying for %d ticks\n", DELAY_TICKS);
     /* 延时20个Tick */
     LOS_TaskDelay(DELAY_TICKS);
     
     /* 输出提示信息 */
     printf("High priority task is awake after delay\n");
     
     /* 挂起自身 */
     printf("High priority task suspending itself\n");
     LOS_TaskSuspend(g_highPriorityTaskID);
     
     /* 被恢复后执行 */
     printf("High priority task resumed and running again\n");
     
     while (1) {
         printf("High priority task running...\n");
         LOS_TaskDelay(1000); /* 防止任务空转 */
     }
 }
 
 /* 低优先级任务函数 */
 VOID LowPriorityTaskEntry(VOID)
 {
     printf("Low priority task started, delaying for %d ticks\n", DELAY_TICKS);
     /* 延时20个Tick */
     LOS_TaskDelay(DELAY_TICKS);
     
     /* 输出提示信息 */
     printf("Low priority task is awake after delay\n");
     
     /* 恢复高优先级任务 */
     printf("Low priority task resuming high priority task\n");
     LOS_TaskResume(g_highPriorityTaskID);
     
     /* 挂起自身 */
     printf("Low priority task suspending itself\n");
     LOS_TaskSuspend(g_lowPriorityTaskID);
     
     while (1) {
         printf("Low priority task running...\n");
         LOS_TaskDelay(1000); /* 防止任务空转 */
     }
 }
 
 /* 实验任务创建函数 */
 VOID TaskScheduleTest(VOID)
 {
     UINT32 ret;
     TSK_INIT_PARAM_S taskParam = {0};
     
     printf("Creating high priority task...\n");
     /* 创建高优先级任务 */
     taskParam.pfnTaskEntry = (TSK_ENTRY_FUNC)HighPriorityTaskEntry;
     taskParam.uwStackSize = TASK_STACK_SIZE;
     taskParam.pcName = "HighPriorityTask";
     taskParam.usTaskPrio = HIGH_TASK_PRIO;
     ret = LOS_TaskCreate(&g_highPriorityTaskID, &taskParam);
     if (ret != LOS_OK) {
         printf("High priority task create failed: 0x%x\n", ret);
         return;
     }
     
     printf("Creating low priority task...\n");
     /* 创建低优先级任务 */
     taskParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LowPriorityTaskEntry;
     taskParam.uwStackSize = TASK_STACK_SIZE;
     taskParam.pcName = "LowPriorityTask";
     taskParam.usTaskPrio = LOW_TASK_PRIO;
     ret = LOS_TaskCreate(&g_lowPriorityTaskID, &taskParam);
     if (ret != LOS_OK) {
         printf("Low priority task create failed: 0x%x\n", ret);
         return;
     }
     
     printf("Task schedule test started\n");
 }
 
 /* 保持原有的接口 RunTaskSample */
 VOID RunTaskSample(VOID)
 {
     UINT32 ret;
     printf("Initializing kernel...\n");
     ret = LOS_KernelInit();
     if (ret == LOS_OK) {
         printf("Kernel initialized successfully\n");
         TaskScheduleTest(); /* 替换为我们的测试函数 */
         printf("Starting scheduler...\n");
         LOS_Start();
     } else {
         printf("Kernel initialization failed: 0x%x\n", ret);
     }
 }