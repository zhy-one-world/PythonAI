import unreal_engine
class test:
    def __init__(self) -> None:
        unreal_engine.find_class("ThirdPersonCharacter_C")
    def begin_play(self):
        uobject.GetPointFromStartToEnd()
    def tick(self,delta_seconds):
        pass