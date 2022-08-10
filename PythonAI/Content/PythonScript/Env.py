import gym
from typing import Tuple, List, Any

"""
文档：https://smlhic47en.feishu.cn/docx/doxcnJ8EPMNlM9I3nZLpWtQVOmf
"""

# 定义action和obs的类型
ActionType = List
ObsType = List

class GameEnv:

    def __init__(self, n_ray: int) -> None:
        """
        初始化游戏环境
        :param n_ray: 射线数量, 用于确定环境状态
        """
        self.n_ray = n_ray
        self.observation_space = gym.spaces.Box(low=0.0, high=1.0, shape=(n_ray, ))
        self.action_space = gym.spaces.MultiDiscrete([2, 3, 3, 3, 3])
        # 游戏其他需要初始化的内容
        ...
        # 游戏其他需要初始化的内容

    def get_obs(self) -> ObsType:
        """
        返回游戏当前状态
        :return: 当前状态
        """
        # 此处生成随机状态用于占位
        return self.observation_space.sample()


    def reset(self, seed: int = 0) -> ObsType:
        """
        重置游戏环境，可以使用seed作为随机种子游戏中一些内容随机化重置
        例如角色在地图中的位置、角色初始的状态等
        :return: 返回初始状态
        """
        # 重置
        ...
        # 重置
        return self.get_obs()

    def step(self, action: ActionType) -> Tuple[ObsType, float, bool, Any]:
        """
        agent与环境进行单步交互，从agent获得action，并在游戏中进行一步。
        :param action: 当前步，角色需要执行的动作
        :return: 动作执行完毕之后，环境和角色发生变化，输出新一步的观测（状态）、当前动作-状态奖励、本局游戏是否完成、其他游戏信息（可以返回None）
        """
        is_jump = action[0] # 是否跳跃
        fb_move = action[1] # 前后移动
        lr_move = action[2] # 左右移动
        sight_ud = action[3] # 视线上下移动
        sight_lr = action[4] # 视线左右移动
        if is_jump:
            # 角色跳跃
            ...
            # 角色跳跃
        if fb_move:
            if fb_move == 1:
                # 角色向前移动
                ...
                # 角色向前移动
            elif fb_move == 2:
                # 角色向后移动
                ...
                # 角色向后移动
        if lr_move:
            if lr_move == 1:
                # 角色向左移动
                ...
                # 角色向左移动
            elif lr_move == 2:
                # 角色向右移动
                ...
                # 角色向右移动
        if sight_ud:
            if sight_ud == 1:
                # 角色视线向上移动
                ...
                # 角色视线向上移动
            elif sight_ud == 2:
                # 角色视线向下移动
                ...
                # 角色视线向下移动
        if sight_lr:
            if sight_lr == 1:
                # 角色视线向左移动
                ...
                # 角色视线向左移动
            elif sight_lr == 2:
                # 角色视线向右移动
                ...
                # 角色视线向右移动

        # next_observation, 由于agent和环境互动之后，改变了环境，此处需要获得最新的状态。
        next_observation = self.get_obs()
        # 奖励, 用于指引 agent 完成任务, 为了方便起见，设定为每一步都给予+1的奖励。
        reward = 1.0
        # 本局游戏是否完成, 方便起见，可以每局游戏运行一定时间就结束，done=True
        done = False
        # 其他需要环境传递给agent的信息, 此处可以忽略
        info = None
        return next_observation, reward, done, info

    def render(self) -> None:
        """
        游戏画面渲染一步, 如果在main.py中不执行该函数，那么就不会渲染游戏画面
        :return: 无返回值
        """
        pass

    def close(self) -> None:
        """
        关闭游戏环境，销毁相关资源
        :return: 无返回值
        """
        pass