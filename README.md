# UE4引擎版本  
  4.27.1  
# VS版本  
VS2019 + update3  
# UE4使用  
  下载UE4源码，选择4.27.1版本。下载地址：https://github.com/EpicGames/UnrealEngine.git  
  下载成功后点击 GenerateProjectFiles.bat 下载辅助性dll  
  dll下载完毕后点击 Setup.bat 生成 UE4.sln  
  编译UE4.sln  
  双击 Engine\Binaries\Win64\UnrealVersionSelector-Win64-Shipping.exe 注册UE4路径  
# 项目使用  
  右键点击 PythonAI\PythonAI.uproject,点击 Switch Unreal Version，在弹出窗口中选择下载好UE4的路径，点击OK，生成PythonAI.sln  
  编译，运行PythonAI.sln  
  在编辑器窗口选择运行，就可以体验demo版本了  
