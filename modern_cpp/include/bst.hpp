/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : RD5678
****************************************************************************/
#ifndef __BST_H_RD5678_ILRD__
#define __BST_H_RD5678_ILRD__

#include <iostream>
#include <memory>
#include <stack>
#include <cassert>

namespace ilrd_5678
{

template <typename T>
class BST
{
public:
	explicit BST();
	~BST();

	BST(const BST& _other) = delete;
	BST& operator=(const BST& _other) = delete;

	BST(BST&& _other);
	BST& operator=(BST&& _other);

	void Insert(T&& data);
	T Find(T data) const;
	bool IsEmpty() const;
	size_t Size() const;
	void ClearTree();

private:

	class Node
	{
	public:
		explicit Node(T data);

		Node& GetLeftChild() const;
		Node& GetRightChild() const;
		Node& GetParent() const;

		T& GetData() const;
		void SetData(const T& data);

		void SetLeft(const Node& left);
		void SetRight(const Node& right);
		void SetParent(const Node& parent);

		void UnlinkRightChild();
		void UnlinkLeftChild();
		void UnlinkParent();

	private:
		T m_data;
		std::unique_ptr<Node> m_left;
		std::unique_ptr<Node> m_right;
		Node *m_parent;
	};

	std::unique_ptr<Node> m_root;

	Node& GetRoot() const;
	void InsertNode(Node *node);
	void ConnectNodes(Node *parent, Node *node);
	bool IsSameNode(Node *node1, Node *node2);
	bool IsLeafNode(Node *node);
	void RemoveNode(Node *parent, Node *node);
	Node *LocateNodeToDel(T data);
};

/*******************************************************************************
 * 								Class BST									   *
 ******************************************************************************/
template <typename T>
BST<T>::BST() : m_root(new Node(0))
{
	//empty Ctor
}

template <typename T>
BST<T>::~BST()
{
	//empty Dtor
}

template <typename T>
BST<T>::BST(BST&& _other)
{
	m_root = std::move(_other.m_root);
	_other.m_root.reset();
}

template <typename T>
typename BST<T>::BST& BST<T>::operator=(BST&& _other)
{
	if (this != &_other)
	{
		m_root.reset(std::move(_other.m_root));
		_other.m_root.reset();
	}
}

template <typename T>
bool BST<T>::IsEmpty() const
{
	return (&(GetRoot()) == nullptr);
}

template <typename T>
void BST<T>::Insert(T&& data)
{
	Node *newNode = new Node(std::forward<T>(data));

	InsertNode(newNode);
}

template <typename T>
T BST<T>::Find(T data) const
{
	T result = m_root->GetData();
	Node *runner = &(GetRoot());

	while (nullptr != runner)
	{
		if (data < runner->GetData())
		{
			runner = &(runner->GetLeftChild());
		}
		else if (data > runner->GetData())
		{
			runner = &(runner->GetRightChild());
		}
		else
		{
			result = runner->GetData();
			break;
		}
	}

	return result;
}

template <typename T>
size_t BST<T>::Size() const
{
	std::stack<Node *> nodeStack;
	size_t counter = 0;

	if (IsEmpty())
	{
		return 0;
	}

	nodeStack.emplace(&GetRoot());

	while (!nodeStack.empty())
	{
		auto currNode = nodeStack.top();
		nodeStack.pop();
		++counter;

		if (nullptr != &(currNode->GetLeftChild()))
		{
			nodeStack.emplace(&(currNode->GetLeftChild()));
		}
		if (nullptr != &(currNode->GetRightChild()))
		{
			nodeStack.emplace(&(currNode->GetRightChild()));
		}
	}

	return counter;
}

template <typename T>
void BST<T>::ClearTree()
{
	std::stack<Node *> nodeStack;

	nodeStack.emplace(&GetRoot());

	while (!nodeStack.empty())
	{
		auto currNode = nodeStack.top();

		if (IsLeafNode(currNode))
		{
			nodeStack.pop();
			RemoveNode(&(currNode->GetParent()), currNode);
		}
		else
		{
			if (nullptr != &(currNode->GetLeftChild()))
			{
				nodeStack.emplace(&(currNode->GetLeftChild()));
			}
			if (nullptr != &(currNode->GetRightChild()))
			{
				nodeStack.emplace(&(currNode->GetRightChild()));
			}
		}
	}
}

/*******************************************************************************
						BST Private Member functions							
*******************************************************************************/
template <typename T>
typename BST<T>::Node& BST<T>::GetRoot() const
{
	return m_root->GetLeftChild();
}

template <typename T>
void BST<T>::InsertNode(Node *node)
{
	Node *parent = m_root.get();
	Node *runner = &(GetRoot());

	while (nullptr != runner)
	{
		parent = runner;
		if (node->GetData() < runner->GetData())
		{
			runner = &(runner->GetLeftChild());
		}
		else
		{
			runner = &(runner->GetRightChild());
		}
	}

	ConnectNodes(parent, node);
}

template <typename T>
void BST<T>::ConnectNodes(Node *parent, Node *node)
{
	if (IsSameNode(parent, m_root.get()))
	{
		parent->SetLeft(*node);
	}
	else
	{
		if (node->GetData() < parent->GetData())
		{
			parent->SetLeft(*node);
		}
		else
		{
			parent->SetRight(*node);
		}
	}

	node->SetParent(*parent);
}

template <typename T>
bool BST<T>::IsSameNode(Node *node1, Node *node2)
{
	return (node1 == node2);
}

template <typename T>
bool BST<T>::IsLeafNode(Node *node)
{
	return (nullptr == &(node->GetLeftChild()) && 
			nullptr == &(node->GetRightChild()) );
}

template <typename T>
void BST<T>::RemoveNode(Node *parent, Node *node)
{
	node->UnlinkParent();

	if (IsSameNode(&(parent->GetLeftChild()), node))
	{
		parent->UnlinkLeftChild();
	}
	if (IsSameNode(&(parent->GetRightChild()), node))
	{
		parent->UnlinkRightChild();
	}

}

template <typename T>
typename BST<T>::Node *BST<T>::LocateNodeToDel(T data)
{
	Node *runner = &(GetRoot());

	while (runner != nullptr)
	{
		if (runner->GetData() > data)
		{
			runner = &(runner->GetLeftChild());
		}
		else if (runner->GetData() < data)
		{
			runner = &(runner->GetRightChild());
		}
		else
		{
			break;
		}
	}

	return runner;
}

/*******************************************************************************
 * 								Class Node									   *
 ******************************************************************************/

/******************************************************************************/
template <typename T>
BST<T>::Node::Node(T data) 
				:	m_data(data), 
					m_left(),
					m_right(),
					m_parent()
{
	//empty Ctor
}

template <typename T>
typename BST<T>::Node& BST<T>::Node::GetLeftChild() const
{
	return *m_left;
}

template <typename T>
typename BST<T>::Node& BST<T>::Node::GetRightChild() const
{
	return *m_right;
}

template <typename T>
typename BST<T>::Node& BST<T>::Node::GetParent() const
{
	return *m_parent;
}

template <typename T>
T& BST<T>::Node::GetData() const
{
	return const_cast<T&>(m_data);
}

template <typename T>
void BST<T>::Node::SetLeft(const Node& left)
{
	m_left.reset(const_cast<Node *>(&left));
} 

template <typename T>
void BST<T>::Node::SetRight(const Node& right)
{
	m_right.reset(const_cast<Node *>(&right));
}

template <typename T>
void BST<T>::Node::SetParent(const Node& parent)
{
	m_parent = const_cast<Node *>(&parent);
} 

template <typename T>
void BST<T>::Node::UnlinkRightChild()
{
	m_right.reset();
}

template <typename T>
void BST<T>::Node::UnlinkLeftChild()
{
	m_left.reset();
}

template <typename T>
void BST<T>::Node::UnlinkParent()
{
	m_parent = nullptr;
}

} // namespace ilrd_5678

#endif /* __BST_H_RD5678_ILRD__ */
