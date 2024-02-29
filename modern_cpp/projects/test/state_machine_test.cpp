#include "state_machine.hpp"

#include <type_traits>

/*
Test for simple close/open state machince
*/

struct CloseEvent : public IEvent{};
struct OpenEvent : public IEvent{};

struct ClosedState;
struct OpenState;

using StateVariant = std::variant<ClosedState, OpenState>;
using OptionalStateVariant = std::optional<StateVariant>;

struct ClosedState
{
    OptionalStateVariant Handle(const IEvent&) const;
    
    void Print() const
    {
        std::cout << "ClosedState\n";
    }
};

struct OpenState
{
    OptionalStateVariant Handle(const IEvent&) const;
    
    void Print() const
    {
        std::cout << "OpenState\n";
    }
};

OptionalStateVariant ClosedState::Handle(const IEvent&) const
{
    std::cout << "Closed -> Open\n";
    return OpenState{};
}

OptionalStateVariant OpenState::Handle(const IEvent&) const
{
    std::cout << "Open -> Closed\n";
    return ClosedState{};
}


int main()
{
    using Door = StateMachine<ClosedState, OpenState>;

    Door simple_door;

    simple_door.HandleEvent(OpenEvent{});
    simple_door.HandleEvent(CloseEvent{});
    
    //std::visit([](const auto state){ state.Print(); }, simple_door.GetState());

    return 0;
}