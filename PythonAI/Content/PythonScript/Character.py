import unreal
import CharacterAction
import SamplingPicture

class Character:
    ue_character = None
    durtime = 0.0
    randomtime = 0.0
    turntime = 0.0
    action_state = 0
    character_action = None
    def __init__(self):
        print("Character init")
        self.character_action = CharacterAction.CharacterAction()
    def jump(self):
        self.ue_character.jump()
        position = self.ue_character.get_actor_location()
        print(position)
    def im_move_position(self):
        self.action_state = -1
        position = unreal.Vector( -1860.0, -1370.0, 222.0 )
        self.ue_character.set_actor_location( position, False, True )
        self.ue_character.on_reset()
        print("im_move_position")
        self.action_state = 0
    def move_forward(self):
        self.ue_character.move_forward(1)
    def move_back(self):
        self.ue_character.move_forward(-1)
    def turn_left(self):
        self.ue_character.move_right(-1)
    def turn_right(self):
        self.ue_character.move_right(1)
    def look_up(self):
        self.ue_character.add_controller_pitch_input(-0.6)
    def look_down(self):
        self.ue_character.add_controller_pitch_input(0.6)
    def look_left(self):
        self.ue_character.add_controller_yaw_input(-0.6)
    def look_right(self):
        self.ue_character.add_controller_yaw_input(0.6)
    def tick(self, delta):
        RangeSize = unreal.Vector2D(800,600)
        SamplingPicture.picture_sampling(RangeSize,delta,2)
        if( self.action_state == 0 ):
            self.character_action.CreateRandomAction()
            self.action_state = 1
            #print("action_state==0111", self.character_action.durtime )
        elif( self.action_state == 1 ):
            #print("action_state==1")
            self.turntime = self.turntime + delta
            if( self.turntime < self.character_action.turn_time ):
                if( self.character_action.dirc == 1 ):
                    self.look_left()
                elif( self.character_action.dirc == 2 ):
                    self.look_right()
            else:
                self.turntime = 0.0
                self.action_state = 2
                self.randomtime = 0.0
        elif( self.action_state == 2 ):
            #print("action_state==2", self.character_action.durtime)
            self.randomtime = self.randomtime + delta
            if( self.randomtime >= self.character_action.durtime ):
                self.randomtime = 0.0
                self.action_state = 0
            else:
                self.move_forward()
                
        #print("timedelta", self.delta )



print("loaded Character!")