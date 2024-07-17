# LM4265 Driver

> 128 X 128 Graphices LCD Module (LM4265) Driver for AT89C51

### 介绍

LM4265 是一个 128*128 的图形液晶显示模块，拥有 20 个引脚，内置字符发生器 CGROM，可以通过编程为用户提供直观、便捷的操作界面。

由于本人在网上根本搜不到 LM4265 的任何相关资料，想要使用该模块甚至连说明书都没有，于是折腾了一个通宵完成了此驱动程序，后续还会继续完善。

经过测试，LM4265 采用的驱动芯片为 [Toshiba T6963C Controller](https://www.manuallib.com/download/T6963C-CONTROLLER-SERIES-LIQUID-CRYSTAL-MODULE-CHINESE-MANUAL.PDF) ，故本项目参考 DS240128-4 驱动程序，完成了 LM4265 驱动程序，希望对大家有所帮助。


### 引脚功能

- 1-Vss : Ground(0V)

- 2-Vdd : Logic supply Voltage(+5V)

- 3-VO : LC drive Vlotage for contrast adjustment

- 4-C/D : Command/Data Selection

- 5-RD : Read, Active Low

- 6-WR : Write, Active Low

- 7-DB0 : Bi-directional data bus line 0

- 8-DB1 : Bi-directional data bus line 1

- 9-DB1 : Bi-directional data bus line 2

- 10-DB1 : Bi-directional data bus line 3

- 11-DB1 : Bi-directional data bus line 4

- 12-DB1 : Bi-directional data bus line 5

- 13-DB1 : Bi-directional data bus line 6

- 14-DB1 : Bi-directional data bus line 7

- 15-CE : Chip enable, Active Low

- 16-RESET : Chip enable, Active Low

- 17-VEE : Negative voltage input for LC drive 

  (Negative voltage output for models with on-board negative voltage generator)

- 18-MD2 : Mode Selection

- 19-FS1 : Terminals for selection of font size

- 20-HALT : Halt Function(H = Normal, L = Stop oscillation)

### 注意

- 已在 Proteus 8 中仿真测试，采用的单片机为 AT89C51

### 参考

- [T6963C 控制器系列液晶模块使用说明书 ](https://www.manuallib.com/download/T6963C-CONTROLLER-SERIES-LIQUID-CRYSTAL-MODULE-CHINESE-MANUAL.PDF)
- [T6963C 中文数据手册完整版](https://www.doc88.com/p-980340066406.html)
- [LM4265 Datasheet](https://datasheet4u.com/download_new.php?id=811391) 
