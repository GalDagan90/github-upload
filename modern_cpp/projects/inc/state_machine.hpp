#ifndef __GAL_DAGAN_PROJ_STATE_MACHINE__
#define __GAL_DAGAN_PROJ_STATE_MACHINE__

#include <iostream>
#include <tuple>
#include <variant>
#include <functional>

/*
Generic State machine templat class
The "State" type should have a member function Called "Handle" that accepts as an argument
an "Event". The state's Handle function should return the new state the machine should change in to
*/

template <typename... States>
class StateMachine
{
private:
    std::tuple<States...> m_states_tup;
    std::variant<States*...> m_curr_state{ &std::get<0>(m_states_tup) };

public:
    template <typename NewState>
    void ChangeState()
    {
        m_curr_state = &std::get<NewState>(m_states_tup);
    }

    auto GetState() const
    {
        return m_curr_state;
    }

    template <typename Event>
    void HandleEvent(const Event& event)
    {
        std::visit([this, &event](const auto state_ptr)
                    {
                        auto new_state = state_ptr->Handle(event);
                        this->ChangeState<decltype(new_state)>();
                    }
                    ,m_curr_state);
    }
};



#endif //__GAL_DAGAN_PROJ_STATE_MACHINE__