import unreal_engine
class test:
    def __init__(self):
        unreal_engine.find_class("ThirdPersonCharacter_C")
    def begin_play(self):
        print("Begin")
        self.uobject.WriteOrRead = False #读写切换，True为写，False为读，放到begin_play执行
        self.uobject.ReadCharacterDataTextPath = "E:\\" #读取路径，放到begin_play执行
        self.uobject.ReadCharacterDataTextName = "CHA.TXT" #读取文件名称，放到begin_play执行
        self.uobject.WriteCharacterDataTextPath = "E:\\" #写入路径，放到begin_play执行
        self.uobject.WriteCharacterDataTextName = "CHA.TXT" #写入文件名称，放到begin_play执行
    def tick(self,delta_seconds):
        pass