import copy
import gym
import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F
from typing import Any
from functools import reduce
from collections import namedtuple
import random
from torch.autograd import Variable

use_cuda = torch.cuda.is_available()
FloatTensor = torch.cuda.FloatTensor if use_cuda else torch.FloatTensor
LongTensor = torch.cuda.LongTensor if use_cuda else torch.LongTensor
ByteTensor = torch.cuda.ByteTensor if use_cuda else torch.ByteTensor
Tensor = FloatTensor

from Env import GameEnv, ObsType, ActionType

class AgentBase:
    def __init__(self, env: GameEnv) -> None:
        self.env = env
        self.observation_space = env.observation_space
        self.observation_dim = int(reduce(lambda x, y: x * y, env.observation_space.shape, 1.0))
        self.action_space = env.action_space
        if isinstance(self.action_space, gym.spaces.Discrete):
            self.action_dim = self.action_space.n
        elif isinstance(self.action_space, gym.spaces.MultiDiscrete):
            self.action_dim = int(reduce(lambda x, y: x * y, self.action_space.nvec, 1.0))

    def get_action(self, obs: ObsType) -> ActionType:
        """
        根据当前观测（状态）进行决策, 输出动作
        :param obs: 来自环境的观测(状态)，一般为 list 类型
        :return:
        """
        return [0, 0, 0, 0, 0]

    def learn(self, *trans) -> None:
        """
        :return:
        """
        pass

class RandomAgent(AgentBase):
    def get_action(self, obs: ObsType):
        return self.action_space.sample()

Transition = namedtuple('Transition', ('obs', 'action', 'reward', 'next_obs'))

class ReplayMemory(object):
    def __init__(self, capacity):
        self.capacity = capacity
        self.memory = []
        self.position = 0
        self.size = 0

    def push(self, *args):
        if len(self.memory) < self.capacity:
            self.memory.append(None)
        self.memory[self.position] = Transition(*args)
        self.position = (self.position + 1) % self.capacity
        self.size = min(self.size + 1, self.capacity)

    def sample(self, batch_size):
        return random.sample(self.memory, batch_size)

    def __len__(self):
        return len(self.memory)

class QNet(nn.Module):

    def __init__(self, layers):
        super(QNet, self).__init__()
        self.layers = nn.ModuleList()
        for layer_index in range(1, len(layers)):
            self.layers.append(nn.Linear(layers[layer_index - 1], layers[layer_index]))
            if layer_index != len(layers) - 1:
                self.layers.append(nn.ReLU())

    def forward(self, obs):
        logits = obs
        for layer in self.layers:
            logits = layer(logits)
        logits = F.softmax(logits, dim=0)
        return logits

class DQNAgent(AgentBase):
    def __init__(self, env: GameEnv, hidden_layers: list = [256, 256]):
        super(DQNAgent, self).__init__(env)
        self.hidden_layers = hidden_layers
        self.buffer_size = 500
        self.batch_size = 100
        self.lr = 0.004
        self.epsilon = 0.9
        self.gamma = 0.9
        self.start_step = 64
        self.target_net_replace_req = 4

        self.q_net = QNet([self.observation_dim] + hidden_layers + [self.action_dim])
        self.q_target_net = copy.deepcopy(self.q_net)
        self.replay_buffer = ReplayMemory(self. buffer_size)

        self.optimizer = torch.optim.Adam(self.q_net.parameters(), lr=self.lr)
        self.loss_func = nn.MSELoss()
        self.target_net_replace_cnt = 0

    def get_action(self, obs: ObsType):
        # 随机采样动作
        if random.random() > self.epsilon:
            action = self.action_space.sample()
            if isinstance(self.action_space, gym.spaces.Discrete):
                self._action = action
            elif isinstance(self.action_space, gym.spaces.MultiDiscrete):
                # MultiDiscrete Action --(by nvec)--> Discrete Action
                self._action = 0
                weight = 1
                for i in range(-1, -len(self.action_space.nvec)-1, -1):
                    self._action += action[i] * weight
                    weight *= self.action_space.nvec[i]
                self._action = int(self._action)
        # 根据 qvalue 选取动作
        else:
            _action_logits = self.q_net(Tensor(obs))
            _action = torch.argmax(_action_logits).item()
            self._action = _action
            if isinstance(self.action_space, gym.spaces.Discrete):
                action = _action
            elif isinstance(self.action_space, gym.spaces.MultiDiscrete):
                # Discrete Action --(by nvec)--> MultiDiscrete Action
                nvec = self.action_space.nvec
                action = [0] * len(nvec)
                i = -1
                while _action > 0.0 and i >= -len(nvec):
                    action[i] = _action % nvec[i]
                    _action //= nvec[i]
                    i -= 1
        return action

    def _learn(self, batch):
        if self.target_net_replace_cnt % self.target_net_replace_req == 0:
            self.q_target_net.load_state_dict(self.q_net.state_dict())
        self.target_net_replace_cnt = (self.target_net_replace_cnt + 1) % self.target_net_replace_req

        # 计算非最终状态的掩码并连接批处理元素s
        non_final_mask = ByteTensor(tuple(map(lambda s: s is not None, batch.next_obs)))
        non_final_next_obses = Variable(torch.cat([s for s in batch.next_obs if s is not None]), volatile=True)
        obs_batch = Variable(torch.cat(batch.obs))
        action_batch = Variable(torch.cat(batch.action))
        reward_batch = Variable(torch.cat(batch.reward))

        # 计算 Q(s_t, a) - 模型计算出 Q(s_t), 然后我们选择某一栏动作执行
        q_value = self.q_net(obs_batch)
        obs_action_values = q_value.gather(1, action_batch.type(LongTensor))

        # 对所有下一状态计算出 V(s_{t+1})
        next_obs_values = Variable(torch.zeros(self.batch_size).type(Tensor))
        next_obs_values[non_final_mask] = self.q_target_net(non_final_next_obses).max(1)[0]
        next_obs_values.volatile = False

        # 计算 Q 的期望值
        expected_obs_action_values = (next_obs_values * self.gamma) + reward_batch
        # 计算 Huber 损失
        loss = F.smooth_l1_loss(obs_action_values, expected_obs_action_values)

        # 优化模型
        self.optimizer.zero_grad()
        loss.backward()
        for param in self.q_net.parameters():
            param.grad.data.clamp_(-1, 1)
        self.optimizer.step()

    def learn(self, *trans):
        _trans = [None, None, None, None]
        # obs and next_obs
        if isinstance(trans[0], np.ndarray):
            _trans[0] = torch.from_numpy(trans[0]).reshape((-1, self.observation_dim))
            _trans[3] = torch.from_numpy(trans[3]).reshape((-1, self.observation_dim))
        elif isinstance(trans[0], (list, tuple, Any)):
            _trans[0] = torch.Tensor(trans[0]).reshape((-1, self.observation_dim))
            _trans[3] = torch.Tensor(trans[3]).reshape((-1, self.observation_dim))
        # action and reward
        _trans[1] = Tensor([self._action]).reshape((-1, 1))
        _trans[2] = Tensor([trans[2]]).reshape((-1, 1))
        trans = tuple(_trans)

        self.replay_buffer.push(*trans)
        if self.replay_buffer.size >= self.batch_size:
            trans_batch = self.replay_buffer.sample(self.batch_size)
            trans_batch = Transition(*zip(*trans_batch))
            self._learn(trans_batch)