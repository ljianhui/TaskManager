# TaskManager
>这是一个基于Linux的任务管理器，类型于Linux系统自带的System Monitor和Windows下的任务管理器。
>
>该任务管理器基于MVC模式，界面和控制层使用Qt5.4开发，代码量比较少。其核心使用标准C/C++和Linux下的系统调用进行开发。
>
>核心代码完全独立于界面和控制层，可以直接从该项目抽离，使用其他的界面框架，根据自己的需要进行开发。
>
>核心代码放在项目中的core目录下。
>
>其运行效果如下：
![Alt 运行效果](/img/processinfo.png)
