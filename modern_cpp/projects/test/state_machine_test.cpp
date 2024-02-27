#include "state_machine.hpp"

#include <type_traits>

/*
Test for simple close/open state machince
*/

struct IEvent{};

struct CloseEvent : public IEvent{};
struct OpenEvent : public IEvent{};

struct ClosedState;
struct OpenState;


struct ClosedState
{
    OpenState Handle(const IEvent&) const;
    
    void Print() const
    {
        std::cout << "ClosedState\n";
    }
};

struct OpenState
{
    ClosedState Handle(const IEvent&) const;
    
    void Print() const
    {
        std::cout << "OpenState\n";
    }
};

OpenState ClosedState::Handle(const IEvent&) const
{
    std::cout << "Closed -> Open\n";
    return OpenState{};
}

ClosedState OpenState::Handle(const IEvent&) const
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
    //std::visit([](const auto state){ state->Print(); }, simple_door.GetState());

    //simple_door.HandleEvent(OpenEvent{});

    return 0;
}