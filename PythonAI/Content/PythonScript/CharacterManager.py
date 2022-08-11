import unreal
import Character

class CharacterManager:
    character_map = {}
    self_character = None
    def __init__( self ):
        print("PCharacterManager init!!")
    def get_all_character():
        print("get_all_character")

def get_self_character():
    print("get_self_character")
    world = unreal.EditorLevelLibrary.get_game_world()
    ue_character = unreal.GameplayStatics.get_player_pawn(world, 0)        
    CharacterManager.self_character = Character.Character()
    CharacterManager.self_character.ue_character = ue_character
    if CharacterManager.self_character is None:
        print("get self character failed!")

print("loaded CharacterManager!")