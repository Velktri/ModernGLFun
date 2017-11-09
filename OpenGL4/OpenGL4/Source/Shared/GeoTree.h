#pragma once
#include <vector>
#include <queue>

namespace GeoTree
{
	template <class T>
	static inline void TreeToArray(T* Root, std::vector<T*> *InArray)
	{
		if (Root == NULL)
		{
			InArray->push_back(NULL);
			return; 
		}

		InArray->push_back(Root);
		TreeToArray(Root->LeftNode, InArray);
		TreeToArray(Root->RightNode, InArray);
	}

	template <class T>
	static inline void StackToTree(std::queue<T*> &InNodeStack, T* &InNode)
	{
		if (InNodeStack.empty() || InNodeStack.front() == NULL) 
		{ 
			InNodeStack.pop();
			return; 
		}

		InNode = InNodeStack.front();
		InNodeStack.pop();
		StackToTree(InNodeStack, InNode->LeftNode);
		StackToTree(InNodeStack, InNode->RightNode);
	}
}