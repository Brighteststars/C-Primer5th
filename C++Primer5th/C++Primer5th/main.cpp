#include <iostream>
#include <string>
#include <vector>
#include <deque>
using namespace std;
//
//struct ListNode
//{
//	int m_nValue;
//	ListNode* m_pNext;
//};
//
//ListNode* ReverseList(ListNode* pHead)
//{
//	ListNode* pred = nullptr;
//	while (pHead)
//	{
//		ListNode* pNext = pHead->m_pNext;
//
//		pHead->m_pNext = pred;
//		pred = pHead;
//		pHead = pNext;
//	}
//	return pred;
//}
//
//ListNode* MergeList(ListNode* pHead1, ListNode* pHead2)
//{
//	if (pHead2 == nullptr)
//		return pHead1;
//	else if (pHead1 == nullptr)
//		return pHead2;
//	ListNode* pMergeHead = nullptr;
//
//	if (pHead1->m_nValue < pHead2->m_nValue)
//	{
//		pMergeHead = pHead1;
//		pMergeHead->m_pNext = MergeList(pHead1->m_pNext, pHead2);
//	}
//	else
//	{
//		pMergeHead = pHead2;
//		pMergeHead->m_pNext = MergeList(pHead1, pHead2->m_pNext);
//	}
//	return pMergeHead;
//
//}
//
//int longestSubstringWithDuplication(const string &str)
//{
//	if (str.length() <= 0)
//		return 0;
//	int curLength = 0, maxLength = 0;
//	int* position = new int[26];
//	for (int i = 0; i < 26; i++)
//		position[i] = -1;
//	for (int i = 0; i < str.length(); i++)
//	{
//		int preIndex = position[str[i] - 'a'];
//		if (preIndex < 0 || i - preIndex > curLength)
//		{
//			++curLength;
//		}
//		else
//		{
//			if (curLength > maxLength)
//				maxLength = curLength;
//			curLength = i - preIndex;
//		}
//		position[str[i] - 'a'] = i;
//	}
//	if (curLength > maxLength)
//		maxLength = curLength;
//
//	delete[] position;
//	return maxLength;
//}
//
//vector<int> maxInWindows(const vector<int>& num, unsigned int size)
//{
//	vector<int> maxInWindows;
//	if (num.size() >= size && size >= 1)
//	{
//		deque<int> index;
//		for (unsigned int i = 0; i < size; i++)
//		{
//			while (!index.empty() && num[i] >= num[index.back()])
//				index.pop_back();
//			index.push_back(i);
//		}
//		for (unsigned int i = size; i < num.size(); i++)
//		{
//			maxInWindows.push_back(num[index.front()]);
//			while (!index.empty() && num[i] >= num[index.back()])
//				index.pop_back();
//			if (!index.empty() && index.front() <= (int)(i - size))
//				index.pop_front();
//			index.push_back(i);
//
//		}
//		maxInWindows.push_back(num[index.front()]);
//	
//	}
//	return maxInWindows;
//}
//
//struct A
//{
//	char c = 'a';
//	int num = 0;
//	short i = 0;
//};
//
//class C
//{
//
//};
int main()
{
	cout << "Hello World\n";
	return 0;
}
