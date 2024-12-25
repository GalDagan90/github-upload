#ifndef __TASK_WRAPPER_GAL_D__
#define __TASK_WRAPPER_GAL_D__

#include <memory>
#include <any>
#include <type_traits>

class Itask
{
    virtual ~Itask(){}
    virtual Itask* clone() const = 0;
    virtual std::any Invoke() = 0;
};

template<typename Functor, typename... Args>
class Task : public Itask
{
public:
    Task(Functor&& f, Args&&.. args) :  m_task{std::forward<Functor>(f)}, 
                                        m_args{std::forward<Args>(args)...}
    {
        //empty ctor;
    }

private:
    Functor m_task;
    std::tuple<Args...> m_args;
    using ReturnType = std::invoke_result_t<Functor>
};

class TaskWrapper
{
public:
    template<typename F, typename... Args>
    TaskWrapper(F&& f, Args&&... args){
        using Functor = std::decay_t<F>;
        using ConcreteTaks = Task<Functor, Args...>;
        m_taskObj = std::make_unique<ConcreteTaks>(std::forward<F>(f), std::forward<Args>(Args)...);
    }


private:
    std::unique_ptr<Itask> m_taskObj;
};




#endif //__TASK_WRAPPER_GAL_D__