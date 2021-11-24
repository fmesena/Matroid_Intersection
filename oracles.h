#ifndef ORACLES_H_
#define ORACLES_H_

using namespace std;

struct Edge {
	int u;
	int v;
};

int ORACLE_CALLS;
const int H = 4;

bool Oracle_Uniform(vector<int> S);
bool Oracle_Uniform_Free(vector<int> S, int free);
bool Oracle_Uniform_Exchangeable(vector<int> S, int a, int b);

int  Rank_PowerSet(vector<int> S);
bool Oracle_PowerSet(vector<int> S);
bool Oracle_PowerSet_Free(vector<int> S, int free);
bool Oracle_PowerSet_Exchangeable(vector<int> S, int a, int b);

bool Oracle_Cycle(vector<int> S);
bool Oracle_Cycle_Free(vector<int> S, int free);
bool Oracle_Cycle_Exchangeable(vector<int> S, int a, int b);

bool Oracle_Colour(vector<int> S);
bool Oracle_Colour_Free(vector<int> S, int free);
bool Oracle_Colour_Exchangeable(vector<int> S, int a, int b);

bool Oracle_Transversal_Left(vector<int> S);
bool Oracle_Transversal_Left_Free(vector<int> S, int free);
bool Oracle_Transversal_Left_Exchangeable(vector<int> S, int a, int b);

bool Oracle_Transversal_Right(vector<int> S);
bool Oracle_Transversal_Right_Free(vector<int> S, int free);
bool Oracle_Transversal_Right_Exchangeable(vector<int> S, int a, int b);

bool Oracle_Transversal_MSA(vector<int> S);
bool Oracle_Transversal_MSA_Free(vector<int> S, int free);
bool Oracle_Transversal_MSA_Exchangeable(vector<int> S, int a, int b);


#endif