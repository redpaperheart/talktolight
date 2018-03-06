#include "Model.h"


// Global static pointer used to ensure a single instance of the class.
Model* Model::m_pInstance = NULL;
Model* Model::getInstance(){
    if (!m_pInstance){ // Only allow one instance of class to be generated.
        m_pInstance = new Model;
        m_pInstance->setup();
    }
    return m_pInstance;
}

void Model::setup()
{
    mCurState = mNextState = STATE_NONE;
}

void Model::setCurState(const State &state)
{
    mCurState = state;
//    signal_curStateChanged.emit();
}

void Model::setNextState(const State &state)
{
    mNextState = state;
//    signal_nextStateChanged.emit();
}

bool Model::isTransitioningStates()
{
    return ( mNextState != STATE_NONE );
}
