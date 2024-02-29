#ifndef __GAL_DAGAN_PROJ_STATE_MACHINE__
#define __GAL_DAGAN_PROJ_STATE_MACHINE__

#include <iostream>
#include <tuple>
#include <variant>
#include <functional>
#include <optional>
#include <type_traits>

struct IEvent
{
    /* events should inheret from this structure  */
};


/*
Generic State machine templat class
The "State" type should have a member function Called "Handle" that accepts as an argument
an "Event". The state's Handle function should return the new state the
machine should change into as an std::optional<std::variant<States...>>.
*/


template <typename... States>
class StateMachine
{
private:
    std::tuple<States...> m_states_tup;
    std::variant<States...> m_curr_state{ std::get<0>(m_states_tup) };

public:
    using StateVariant = std::variant<States...>;
    using OptionalStateVariant = std::optional<StateVariant>;

    template <typename NewState>
    void ChangeState()
    {
        m_curr_state = std::get<NewState>(m_states_tup);
    }

    auto GetState() const
    {
        return m_curr_state;
    }

    template <typename Event>
    void HandleEvent(const Event& event)
    {
        //Handle event
        auto new_state = std::visit([this, &event](const auto state) -> OptionalStateVariant
                                    {
                                        return state.Handle(event);
                                    }
                                    ,m_curr_state);

        m_curr_state = std::move(new_state.value());
    }
};



#endif //__GAL_DAGAN_PROJ_STATE_MACHINE__