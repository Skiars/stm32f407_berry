# stm32f407_berry

### 使用方法

1. 安装 Git 客户端（[Git for Windows](https://git-scm.com/download/win)）

2. 在 Git Bash 中执行以下命令：

   ``` bash
   git clone https://github.com/skiars/stm32f407_berry.git
   git submodule init
   git submodule update
   ```


3. 进入 *stm32f407_berry/MDK-ARM* 目录，使用 Keil 打开 *stm32f407_berry.uvprojx* 工程文件
4. 编译、下载程序

### 注意事项

1. 必须使用 64 位的 Windows 系统（*MDK-ARM/map_build.exe* 位 64 位程序）
2. 如果直接在 GitHub 上下载源代码，请手动将 Berry 源代码（https://https://github.com/Skiars/berry）文件夹放到 *Middlewares* 目录下，此时 Berry 解释器源代码的目录结构为 *Middlewares/berry*。
3. STM32 开发板的淘宝链接为 https://item.taobao.com/item.htm?spm=a1z10.3-c-s.w4002-21223910208.10.7a9c6a4bQLJHtB&id=585238891575，宝贝页面有开发板的资料（主要是原理图）