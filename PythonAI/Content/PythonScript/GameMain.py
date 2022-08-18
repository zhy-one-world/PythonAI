import unreal
import Character
import CharacterManager
import CharacterAction
import importlib
import gym
from Agent import RandomAgent, DQNAgent
from Env import GameEnv
import random

"""
文档：https://smlhic47en.feishu.cn/docx/doxcnJ8EPMNlM9I3nZLpWtQVOmf
"""
#if __name__ == '__main__':
class GameMain:
    uecall_delta = 0
    uecall_targettime = 0
    def uecall_tick(delta):     
        if CharacterManager.CharacterManager.self_character is None:
            return
        CharacterManager.CharacterManager.self_character.tick(delta)

        #GameMain.uecall_delta = GameMain.uecall_delta + float(delta)
        #if GameMain.uecall_delta > GameMain.uecall_targettime:
        #    print("tick", GameMain.uecall_delta )
        #    GameMain.uecall_delta = GameMain.uecall_delta - GameMain.uecall_targettime
            #randomval = CharacterAction.CreateRandomAction()
            #GameMain.uecall_targettime = randomval[2]
        #else:
        #    CharacterManager.CharacterManager.self_character.tick(delta)

    def test1():
        print("test1")
    def main():
        CharacterManager.get_self_character()
        print(CharacterManager.CharacterManager.self_character)   
        # episode 次数， 一般来说一局游戏为一个 episode， 训练过程需要进行多局游戏
        episode_num = 1000
        n_ray = 100 # n_ray = 100 or any other number
        # 初始化游戏环境
        game_env = GameEnv(n_ray)
        # 初始化agent，用于控制游戏角色
        agent = RandomAgent(game_env) # or agent = DQNAgent(game_env)
        #print(agent)
        for i in range(episode_num):
            # 重置游戏状态
            #print('iaa=' , i )
            observation = game_env.reset()
            episode_reward = 0.0
            # 游戏循环，一步一步进行，直到本局游戏结束（done==True）
            #while True:
            if True:
                # 渲染游戏画面， 可以选择不渲染
                #print("handle game_env!")
                game_env.render()
                # agent 根据游戏当前观测 observation 进行决策， 返回需要角色执行的动作
                action = agent.get_action(observation)
                # 游戏使用角色动作推进一步，返回新一步的游戏观测、奖励、游戏结束标志和其他相关信息
                next_observation, reward, done, info = game_env.step(action)

                # 累加每步奖励到本局游戏的总奖励中
                episode_reward += reward
                agent.learn(observation, action, reward, next_observation)

                #if done:
                #    break
                observation = next_observation
            #print(f"episode: {i}, reward: {episode_reward}")
        game_env.close()
        print("run GameMain.main")

    def game_end():
        print("game_end")
        #处理获取最终奖励逻辑
        world = unreal.EditorLevelLibrary.get_game_world()
        #game_mode = world.get_auth_game_mode()
        CharacterManager.CharacterManager.self_character.im_move_position()#重新开始


GameMain.main()
print("loaded GameMain")