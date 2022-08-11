import random

def CreateRandomAction():
    dirc = random.randint( 1, 4 )  # 四方向
    angle = random.random()         # 转动角度
    time11 = random.uniform(1,4) #移动秒数
    return (dirc,angle,time11)