import unreal
class ControlledTest:
    def __init__(self):
        self.world = unreal.EditorLevelLibrary.get_game_world()
        self.character = unreal.GameplayStatics.get_player_pawn(self.world, 0)
    def jump(self):
        self.character.jump()
        position = self.character.get_actor_location()
        print(position)
    def move_forward(self):
        self.character.move_forward(1)
    def move_back(self):
        self.character.move_forward(-1)
    def turn_left(self):
        self.character.move_right(-1)
    def turn_right(self):
        self.character.move_right(1)
    def look_up(self):
        self.character.add_controller_pitch_input(-0.6)
    def look_down(self):
        self.character.add_controller_pitch_input(0.6)
    def look_left(self):
        self.character.add_controller_yaw_input(-0.6)
    def look_right(self):
        self.character.add_controller_yaw_input(0.6)

Instance = ControlledTest()
print("Load ControlledTest Success")
