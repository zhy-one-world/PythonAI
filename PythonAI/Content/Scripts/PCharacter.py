import importlib
import PCharacterAutoController
importlib.reload(PCharacterAutoController)
import unreal_engine
importlib.reload(unreal_engine)
from unreal_engine.structs import MyStruct

class Character:
    ue_character_instance = None
    CharacterAutoController = PCharacterAutoController.PCharacterAutoController()
    action_step = 0
    time1 = 0
    time2 = 0
    time3 = 0
    time4 = 0
    def move_forward(self):
        if (self.ue_character_instance == None):
            return
        self.ue_character_instance.MoveForward(1)
    def move_back(self):
        if (self.ue_character_instance == None):
            return
        self.ue_character_instance.MoveForward(-1)
    def move_left(self):
        if (self.ue_character_instance == None):
            return
        self.ue_character_instance.MoveRight(-0.6)
    def move_right(self):
        if (self.ue_character_instance == None):
            return
        self.ue_character_instance.MoveRight(0.6)
    def jump(self):
        if (self.ue_character_instance == None):
            return
        self.ue_character_instance.Jump()
    def look_up(self):
        if (self.ue_character_instance == None):
            return
        self.ue_character_instance.LookUpAtRate(0.6)
    def look_down(self):
        if (self.ue_character_instance == None):
            return
        self.ue_character_instance.LookUpAtRate(-0.6)
    def look_left(self):
        if (self.ue_character_instance == None):
            return
        self.ue_character_instance.TurnAtRate(-0.6)
    def look_right(self):
        if (self.ue_character_instance == None):
            return
        self.ue_character_instance.TurnAtRate(0.6)
    def get_sight(self):
        return self.ue_character_instance.ColorDateArr
    def get_actor_location(self):
        return self.ue_character_instance.GetActorLocation()
    def get_actor_velocity(self):
        return self.ue_character_instance.GetVelocity()
    def get_actor_speed(self):
        return self.ue_character_instance.GetActorSpeed()
    def get_camera_vector(self):
        return self.ue_character_instance.GetCameraForwardVector()
    def action(self,delta_seconds):
        if(self.action_step ==0):
            self.random_action = self.CharacterAutoController.random_action()
            self.random_time = self.CharacterAutoController.random_time()
            self.is_jump = self.random_action[0]
            self.fb_move = self.random_action[1]
            self.lr_move = self.random_action[2]
            self.ud_sight = self.random_action[3]
            self.lr_sight = self.random_action[4]
            self.time_move = self.random_time[0]
            self.time_turn = self.random_time[1]
            self.action_step = 1
        if(self.action_step ==1):
            if(self.is_jump == 1):
                self.jump()
                self.is_jump = 0
            if(self.fb_move != 0):
                if(self.fb_move == 1):
                    self.move_forward()
                else:
                    self.move_back()
                self.time1 +=delta_seconds
                if(self.time1>self.time_move):
                    self.fb_move = 0
                    self.time1 = 0
            if(self.lr_move !=0):
                if(self.lr_move ==1):
                    self.move_left()
                else:
                    self.move_right()
                self.time2 +=delta_seconds
                if(self.time2>self.time_move):
                    self.lr_move = 0
                    self.time2 = 0
            if(self.ud_sight !=0):
                if(self.ud_sight ==1):
                    self.look_up()
                else:
                    self.look_down()
                self.time3 +=delta_seconds
                if(self.time3>self.time_turn):
                    self.ud_sight = 0
                    self.time3 = 0
            if(self.lr_sight !=0):
                if(self.lr_sight ==1):
                    self.look_left()
                else:
                    self.look_right()
                self.time4 +=delta_seconds
                if(self.time4>self.time_turn):
                    self.lr_sight = 0
                    self.time4 = 0
            if((self.is_jump+self.fb_move+self.lr_move+self.ud_sight+self.lr_sight) == 0):
                self.action_step =0
    def tick(self,delta_seconds):
        self.action(delta_seconds)