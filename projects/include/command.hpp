/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : ol108
****************************************************************************/
#ifndef __COMMAND_H_RD5678_ILRD__
#define __COMMAND_H_RD5678_ILRD__

namespace framework
{

class ICommand
{
public:
	virtual ~ICommand() = 0;
	virtual void operator()() = 0;
};


class ReadCommand : public ICommand
{

};

class WriteCommand : public ICommand
{

};

ICommand *ReadBuilder();
ICommand *WriteBuilder();

} // namespace framework

#endif /* __COMMAND_H_RD5678_ILRD__ */
