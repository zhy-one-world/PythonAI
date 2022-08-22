import unreal
import CharacterManager
RangeSize = unreal.Vector2D(800,600)
def get_color_date_arr():
      return CharacterManager.CharacterManager.self_character.ue_character.picture_sampling(RangeSize)
print("Load SamplingPictureSuccess")      
      