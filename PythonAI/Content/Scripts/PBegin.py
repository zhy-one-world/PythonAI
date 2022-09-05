import importlib
import PCharacterManager
importlib.reload(PCharacterManager)
import PCharacter
importlib.reload(PCharacter)
class Begin:
    character = None
    time = 0
    def __init__(self):
        print("Begin")
        self.character = PCharacter.Character()
    def begin_play(self):
        self.character.ue_character_instance = self.uobject.get_owner()
        PCharacterManager.CharacterManager.character_list.append(self.character)
    def tick(self,delta_seconds):
        self.character.tick(delta_seconds)