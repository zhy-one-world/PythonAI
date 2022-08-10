from Agent import RandomAgent, DQNAgent
from Env import GameEnv
import gym
import unreal

"""
文档：https://smlhic47en.feishu.cn/docx/doxcnJ8EPMNlM9I3nZLpWtQVOmf
"""

#if __name__ == '__main__':
def main():
    # episode 次数， 一般来说一局游戏为一个 episode， 训练过程需要进行多局游戏
    episode_num = 10
    n_ray = 100 # n_ray = 100 or any other number
    # 初始化游戏环境
    game_env = GameEnv(n_ray)
    # 初始化agent，用于控制游戏角色
    agent = RandomAgent(game_env) # or agent = DQNAgent(game_env)
    print(agent)
    for i in range(episode_num):
        # 重置游戏状态
        print('i=' , i )
        observation = game_env.reset()
    #    episode_reward = 0.0
        # 游戏循环，一步一步进行，直到本局游戏结束（done==True）
    #    while True:
            # 渲染游戏画面， 可以选择不渲染
    #        game_env.render()
            # agent 根据游戏当前观测 observation 进行决策， 返回需要角色执行的动作
    #        action = agent.get_action(observation)
            # 游戏使用角色动作推进一步，返回新一步的游戏观测、奖励、游戏结束标志和其他相关信息
    #        next_observation, reward, done, info = game_env.step(action)

            # 累加每步奖励到本局游戏的总奖励中
    #        episode_reward += reward
    #        agent.learn(observation, action, reward, next_observation)

    #        if done:
    #            break
    #        observation = next_observation
    #    print(f"episode: {i}, reward: {episode_reward}")
    #game_env.close()
    print("run main")
main()
print("loaded main")