#ifndef __GAL_DAGAN_PROJ_ATM_STATE_MACHINE__
#define __GAL_DAGAN_PROJ_ATM_STATE_MACHINE__

#include "state_machine.hpp"



namespace ATM_States
{
    
struct Idle;
struct GetPinNum;
struct VerifyPin;
struct AccountActions;
struct CheckBalance;
struct GetWithdrawl;
struct ConfirmWithdrawl;
struct Done;

using StateVariant = std::variant<ATM_States::Idle,
                                ATM_States::GetPinNum,
                                ATM_States::VerifyPin,
                                ATM_States::AccountActions,
                                ATM_States::CheckBalance,
                                ATM_States::GetWithdrawl,
                                ATM_States::ConfirmWithdrawl,
                                ATM_States::Done
                                >;

using OptionalStateVariant = std::optional<StateVariant>;

} // namespace ATM_States


using ATM_machine = StateMachine<ATM_States::Idle,
                                ATM_States::GetPinNum,
                                ATM_States::VerifyPin,
                                ATM_States::AccountActions,
                                ATM_States::CheckBalance,
                                ATM_States::GetWithdrawl,
                                ATM_States::ConfirmWithdrawl,
                                ATM_States::Done
                                >;

class ATM
{
private:
    ATM_machine m_atm_machine;
public:
    ATM(/* args */);
    ~ATM();
};




#endif //__GAL_DAGAN_PROJ_ATM_STATE_MACHINE__