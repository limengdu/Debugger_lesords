#include "StateManager.h"

StateManager* StateManager::s_instance = 0;

StateManager::StateManager() {
    // 初始化状态数组
    for (int i = 0; i < MAX_STATES; ++i) {
        m_states[i] = 0;
    }
}

StateManager::~StateManager() {
    // 清理状态（如果它们是动态分配的）
    for (int i = 0; i < MAX_STATES; ++i) {
        delete m_states[i];
        m_states[i] = 0;
    }
}

StateManager* StateManager::getInstance() {
    if (!s_instance) {
        s_instance = new StateManager();
    }
    return s_instance;
}

void StateManager::releaseInstance() {
    delete s_instance;
    s_instance = 0;
}

bool StateManager::registerState(State* state) {
    if (!state) {
        return false;
    }
    
    int id = state->getID();
    if (id <= 0 || id > MAX_STATES) {
        return false;
    }
    
    // 已存在则替换
    if (m_states[id - 1]) {
        delete m_states[id - 1];
    }
    
    m_states[id - 1] = state;
    return true;
}

State* StateManager::getState(int stateId) const {
    if (stateId <= 0 || stateId > MAX_STATES) {
        return 0;
    }
    return m_states[stateId - 1];
}

int StateManager::getStateCount() const {
    int count = 0;
    for (int i = 0; i < MAX_STATES; ++i) {
        if (m_states[i]) {
            ++count;
        }
    }
    return count;
}
