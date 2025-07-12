/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode *new_list = nullptr;
        while ( head != nullptr){
            ListNode *next_node = head->next;
            head->next= new_list;
            new_list = head;
            head = next_node;
        }
        return new_list;

    }
};
