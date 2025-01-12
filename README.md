
# Ai8051u 示例代码

本仓库包含了一系列基于 Ai8051u 芯片的测试程序，旨在帮助开发者熟悉该芯片的功能和开发流程。

## 项目列表

- **001_P1口LED流水灯HID模式**: 演示如何在 HID 模式下使用 P1 口控制 LED 实现流水灯效果。
- **002_P1口delay led闪烁HID模式**: 演示在 HID 模式下使用延时函数控制 P1 口的 LED 闪烁。
- **003_P1口led printf发送回复**: 演示通过 `printf` 函数发送数据，并使用 P1 口的 LED 显示回复状态。
- **004_P1口led 按键控制**: 演示如何使用按键输入控制 P1 口的 LED 状态。
- **005_led_key_smg**: 综合示例，演示 LED、按键和数码管的协同工作。
- **006_tim_定时器任务调度**: 演示如何使用定时器实现简单的任务调度。
- **007_GPIO控制**: 演示如何配置和控制 GPIO 引脚。
- **008_key控制**: 演示按键输入的检测和处理。
- **009_smg显示**: 演示如何驱动数码管显示数字。
- **010_adc_key**: 演示如何使用 ADC 读取按键输入。
- **011_lcd_st7735**: 演示如何驱动 ST7735 LCD 显示屏。

## 快速开始

1. **环境准备**:
   - 安装 [Keil C51](https://www.keil.com/c51/) 开发环境。
   - 获取并安装 Ai8051u 芯片的相关驱动和库文件。

2. **克隆仓库**:
   ```bash
   git clone https://github.com/D77go77/Ai8051u_demo.git
   ```

3. **打开项目**:
   - 在 Keil C51 中，打开相应的项目文件（`.uvproj`），查看和编辑代码。

4. **编译和下载**:
   - 在开发环境中编译项目，确保无错误。
   - 使用编程器将编译后的固件下载到 Ai8051u 开发板。

5. **运行和验证**:
   - 运行下载的程序，根据项目功能验证其正确性，例如观察 LED 闪烁、数码管显示或 LCD 显示等。

## 资源

- [Ai8051u 官方文档](https://www.stcaimcu.com/): 包含数据手册、参考手册等详细资料。
- [Keil C51 官方网站](https://www.keil.com/c51/): 提供开发环境的下载和使用指南。

## 许可证

本项目基于 GPL-3.0 许可证进行分发。有关详细信息，请参阅 [LICENSE](./LICENSE) 文件。

```
