class Solution {
public:
    vector<string> generateParenthesis(int n) {
        vector<string> s;
        dfs(0,0,"",n,s);
        return s;
    }
private:
    void dfs(int openN,int closeN,string s,int n,vector<string>& res){
           if(openN + closeN == n*2){ // if we have used all parentheses
                res.push_back(s);
                return;
           }
           if(openN<n){ // add an open parenthesis 
                dfs(openN+1,closeN,s+"(",n,res);
           }
           if(closeN<openN){ // add a close parenthesis 
                dfs(openN,closeN+1,s+")",n,res);
           }
    }
};
