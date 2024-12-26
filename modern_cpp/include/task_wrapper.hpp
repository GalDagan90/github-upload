#ifndef __TASK_WRAPPER_GAL_D__
#define __TASK_WRAPPER_GAL_D__

#include <memory>
#include <any>
#include <type_traits>

class ITask
{
public:
    virtual ~ITask(){}
    virtual ITask* Clone() const = 0;
    virtual std::any Invoke() = 0;
};

template<typename Functor, typename... Args>
class Task : public ITask
{
public:
    template<typename FunctorFwd>
    Task(FunctorFwd&& f, Args&&... args) :  m_task{std::forward<FunctorFwd>(f)}, 
                                            m_args{std::forward<Args>(args)...}
    {
        //empty ctor;
    }

    Task* Clone() const override
    {
        return new Task(*this);
    }

    std::any Invoke() override
    {
        return nullptr;
    }


private:
    Functor m_task;
    std::tuple<Args...> m_args;
    using ReturnType = std::invoke_result_t<Functor, Args...>;
};



class TaskWrapper
{
public:
    TaskWrapper() = default;

    //Forwarding Constructor
    template<typename F, typename... Args,
            typename = std::enable_if_t<std::is_invocable_v<F, Args...>>>
    TaskWrapper(F&& f, Args&&... args)
    {
        using Functor = std::decay_t<F>;
        using ConcreteTask = Task<Functor, Args...>;
        m_taskObj = std::make_unique<ConcreteTask>(std::forward<F>(f), std::forward<Args>(args)...);
    }

    //Copy Constructor
    TaskWrapper(const TaskWrapper& other)
    {
        if (other.m_taskObj != nullptr)
            m_taskObj = std::unique_ptr<ITask>(other.m_taskObj->Clone());
    }

    //Copy assignment
    TaskWrapper& operator=(const TaskWrapper& other)
    {
        if (this != &other)
        {
            TaskWrapper temp(other);
            std::swap(temp.m_taskObj, this->m_taskObj);
        }
        return *this;
    }

private:
    std::unique_ptr<ITask> m_taskObj;
};




#endif //__TASK_WRAPPER_GAL_D__