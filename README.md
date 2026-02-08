# STM32 BLDC Closed-Loop Speed Control (PID)

## 📖 项目简介 | Introduction
本项目是一个基于 **STM32** 微控制器的无刷直流电机（BLDC）**速度闭环控制**工程。
在开环换相的基础上，引入了 **PID 控制算法**（Proportional-Integral-Derivative），通过霍尔传感器反馈实际转速，自动调节 PWM 占空比，从而实现电机在负载变化时保持转速恒定。

> **适用课程**：张飞实战电子"扬帆起航"电机控制课程案例
> **当前版本**：V1.0.0 (Closed-Loop Demo)
> **作者**：ZhaoYun (ZFSZDZ)

---

## 🛠️ 硬件平台 | Hardware Setup

### 1. 微控制器 (MCU)
* **目标型号**：STM32F103C8T6 (或 STM32G030C8Tx)
    * *⚠️ 注意：当前代码库中的寄存器配置 (`Initialize.c`) 是基于 **STM32G030** 编写的。如需在 STM32F103 上运行，请修改 `Initialize.c` 中的时钟和 GPIO 初始化代码。*

### 2. 电机参数
* **类型**：三相无刷直流电机 (3-Phase BLDC)
* **额定电压**：24V (根据实际驱动板决定)
* **传感器**：霍尔传感器 (Hall Sensors, 120° 相位差)
* **极对数**：4 (需在 `UserParams.h` 中根据实际电机修改)

### 3. 驱动电路
* **驱动方式**：三相全桥逆变电路 (MOSFETs + Gate Drivers)
* **调制方式**：H-PWM-L-ON (上管 PWM 调制，下管恒通)

---

## 🔌 引脚定义 | Pin Definitions

以下引脚定义基于代码中的定时器配置：

| 模块 (Module) | 功能 (Function) | 引脚 (Pin) | 说明 |
| :--- | :--- | :--- | :--- |
| **Hall Sensors** | **Hall A** | **PA6** (TIM3_CH1) | 霍尔信号输入 |
| | **Hall B** | **PA7** (TIM3_CH2) | |
| | **Hall C** | **PB0** (TIM3_CH3) | |
| **Inverter (PWM)** | **UH** (High Side) | **PA8** (TIM1_CH1) | U 相上桥臂 |
| | **VH** (High Side) | **PA9** (TIM1_CH2) | V 相上桥臂 |
| | **WH** (High Side) | **PA10** (TIM1_CH3) | W 相上桥臂 |
| | **UL** (Low Side) | **PB13** (TIM1_CH1N)| U 相下桥臂 |
| | **VL** (Low Side) | **PB14** (TIM1_CH2N)| V 相下桥臂 |
| | **WL** (Low Side) | **PB15** (TIM1_CH3N)| W 相下桥臂 |
| **Control** | **Potentiometer**| **PA5** (ADC_IN5) | 电位器调速 |
| **Communication** | **UART TX** | **PB6** | 串口发送数据 |
| | **UART RX** | **PB7** | 串口接收指令 |

---

## ⚙️ 软件功能 | Software Features

### 1. 闭环控制逻辑 (PID)
本项目核心在于 `PID.c` 模块。
* **反馈源**：通过定时器捕获霍尔信号的变化周期，计算出电机的实际转速。
* **控制量**：PID 计算输出 PWM 占空比 (`Duty`)。
* **控制目标**：使 `ActualSpeed` (实际转速) 快速稳定地跟随 `TargetSpeed` (目标转速)。

### 2. 状态机 (FSM)
系统通过 `BLDC_StateMachine` 管理电机状态：
1.  **Stop**: 停机状态。
2.  **Charge**: 自举电容预充电。
3.  **Start**: 开环启动（强拖）。
4.  **Run (Loop)**: **切入闭环模式**，PID 开始介入调节。

### 3. 关键文件说明
* `Source/PID.c`: PID 算法实现（误差计算、P/I/D 项累加、输出限幅）。
* `Source/BLDC.c`: 霍尔换相逻辑与状态机。
* `Source/Initialize.c`: 寄存器级的外设初始化。

---

## 🚀 移植指南 (G030 -> F103)

由于代码库默认为 STM32G030，若使用 STM32F103，请修改以下宏定义或初始化代码：
1.  **时钟配置**: 将 `RCC->IOPENR` (G0) 修改为 `RCC->APB2ENR` (F103) 以开启 GPIO 时钟。
2.  **GPIO 配置**: 将 `MODER/OSPEEDR` (G0) 修改为 `CRL/CRH` (F103) 寄存器配置。
3.  **Flash 等待**: F103 的 Flash Latency 配置与 G0 不同。

---

## 📜 版权信息 | License
Copyright (c) 2024 ZFSZDZ. All rights reserved.
