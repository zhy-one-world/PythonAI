import random


class CharacterAction:
    dirc = 0
    turn_time = 0.0
    durtime = 0.0
    def CreateRandomAction(self):
        print("CreateRandomAction")
        self.dirc = random.randint( 1, 2 )  # 四方向
        self.turn_time = random.uniform(0.3, 0.6)       # 转动角度
        self.durtime = random.random() * 5 #移动秒数


print("loaded CharacterAction")