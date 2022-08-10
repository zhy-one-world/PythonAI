# 项目名称: PythonAI
-----------------------------------------------------
## 说明: PythonAI 是一个使用Python做算法脚本，附加在UE4游戏中的 AI工具， 可以通过Python脚本来获取数据，进行训练，在拿到训练数据后，在游戏中使用AI算法，进行角色人物化的一些行为模拟 。
<br>

## 人员相关

--------------
<br>

---------------------------
### 使用平台 
#### Unreal Editor(下边简称:UE)
  1. 引擎版本 4.27.1 
  2. 引擎自带python解释器版本3.7.7
  3. python库 Gym
  4. python库 Torch


### 环境配置
#### 如何使用UE中的Python运行
1. 打开UE中菜单栏中编辑,在下拉菜单中找到插件,点击，打开插件UI
2. 在插件UI中，搜索栏目中查找python字，回车后，出现三个插件。
3. 三个插件都进行打勾
4. 打勾后，即可引入了python插件。
5. 打开UE中菜单栏中编辑,在下拉菜单中找到项目设置,点击，打开设置UI
6. 在左边设置列表中，找到插件1级标题，在下边找到Python二级标题,并且单击。
7. 在其它路径中 如果为0数组元素时，则使用+号，增加一行，如果已有一行，则到那行中，点击后边的...进行路径选择，一般直接选择Content/PythonScript。
8. 在其它路径下边有个小箭头，点击后，可看到开发者模式，进行打勾。

### 如何在UE中引擎Python库
1. （可选)在环境变量中，增加UE的Python执行器，一般位置在.引擎安装目录的\Epic Games\UE_4.27\Engine\Binaries\ThirdParty\Python3\Win64下
2. 装完后，在系统命令窗口或者是当前Python命名窗口中，执行: python -m pip install [库名称]

### 如何在UE中运行Python
1. 在工程创建后,可以看到有__pychche__和init_unreal.py的文件。
2. 在init_unreal.py是初始化中，要引入的python脚本，在引入后，需要重启ue4工程，才可以使用引入后的脚本


### 如何查看UE相关的api
1. 可以在https://docs.unrealengine.com/5.0/en-US/PythonAPI/查询相关的api函数
2. 也可以在工程的 例:PythonAI工程名  PythonAI\Intermediate\PythonStub\unreal.py查询相关的api函数,该文件中主要存放着UE封闭过的API
3. 具体用法， 一般使用 类名.函数( 对象 )的方式去调用，例如果获取角色的当前位置可以使用以下代码
```python
  import unreal
  world = unreal.EditorLevelLibrary.get_game_world()
  character = unreal.GamePlayStatic.get_player_pawn(world,0)
  position = character.get_actor_location()
  print(position)
```
4. 带Editor的类或者名称，一般是编辑器模式下，带Engine一般指运行时模式下，暂时，都可以混用，等以后熟悉后，将会模式隔离。
