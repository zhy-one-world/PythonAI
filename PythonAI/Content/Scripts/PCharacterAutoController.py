import random
class PCharacterAutoController:
    def __init__(self):
        print("获得控制器")
    def random_action(self):
        self.action_list = [0]*5
        for num in range(0,len(self.action_list)):
            if (num == 0):
                self.action_list[num] = random.randint(0,1)
            else:
                self.action_list[num] = random.randint(0,2)
            
        return self.action_list
    def random_time(self):
        self.time_move = random.uniform(2,5)
        self.time_turn = random.uniform(0.3,1.0)
        self.randomtime = [self.time_move,self.time_turn]
        return self.randomtime
        