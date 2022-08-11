class Character:
    ue_character = None
    def __init__(self):
        print("Character init")
    def jump(self):
        self.ue_character.jump()
        position = self.ue_character.get_actor_location()
        print(position)
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
        self.ue_character.move_forward(1)



print("loaded Character!")