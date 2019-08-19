#include<iostream>
#include<conio.h>
#include<vector>
#include<stack>
#include<string>
#include<fstream>
#include<algorithm>
using namespace std;

#define LIMIT 500                  //Limit of the vectors/persons
typedef int numtype;              //data type used for numeric
#define vec_num vector<numtype>  
#define f first
#define s second
#define nns pair<pair<numtype,numtype>,string> 

//Globals
numtype NodeID_global=-1;
numtype RootID_global=-1;
numtype FamilyID=-1;

struct Person;
class Family;

vector<Person*> NodeID(LIMIT);             //Stores address of corresponding nodeid
vector<numtype> FamilyRoots;               //Stores nodeID of the roots of a family

//Person's details
struct Person{                         
       
       string name;
       string dob;
       bool gender,dead;                        //0-Female // 1-Male
       numtype nodeid,rootid;
       
       numtype father;                    //nodeid of Father
       numtype mother;
       
       vector<numtype> partner;           //wives and their corresponding nodeid's
       vector<numtype> sibling;
       vector<numtype> children;
};

//---------------------------HELPER FUNCTIONS-----------------------------//

//Gives new NodeID
numtype new_nodeid()
{
        NodeID_global++;
        return NodeID_global-1;
} 

numtype new_familyid()
{
        FamilyID++;
        return FamilyID-1;
}
        
//Creates new person and returns the nodeid of the person
numtype new_Person(string name,string dob,bool gender,bool dead,numtype father_nodeid,numtype mother_nodeid)
{
     Person *nn = new Person;
     
     nn->name=name;
     nn->dob=dob;
     nn->dead=dead;
     nn->gender=gender;
     
     nn->father = father_nodeid;
     nn->mother = mother_nodeid;
     nn->nodeid = new_nodeid();
     
     if(father_nodeid==-1 && mother_nodeid==-1)
     nn->rootid=nn->nodeid;
     
     NodeID[nn->nodeid]=nn;
     
     return nn->nodeid;
}

//Calculates number of Dashes in pattern
int countDash(string pattern)
{
    int i=0;
    while(pattern[i]=='-') {i++;}
    
    return i;
}

//Gives relation between current and previous person
//0-Root   1-Wife     2-Child    3-Sibling    4-Subtree completed   -1 Invalid input
int Relation(string pattern,const stack<nns > temp_stack)
{
    if(pattern[0]=='.') return 0;             //Root of the family
    if(pattern[0]=='+') return 1;             //Wife
    
    int curr_dash = countDash(pattern);
    int prv_dash;
    
    if(!temp_stack.empty())
    {  
        nns p = temp_stack.top();
        string prv_patt = p.s;
        prv_dash = countDash(prv_patt);
        
    }
    
    if(curr_dash==prv_dash+1) return 2;

    if(curr_dash==prv_dash) {return 3;}
    
    if(curr_dash<prv_dash) return 4;

    return -1;
}

void copyBackStack(stack<nns> sibling_stack,stack<nns > &temp_stack)
{
     while(!sibling_stack.empty())
     {
          nns top = sibling_stack.top();
          temp_stack.push(top);
          sibling_stack.pop();
     }
}

//------------------------CLASSES---------------------------------------//

class Family{
      
      numtype rootid,familyID,total_members,total_dead,total_male,total_female; Person *root;
      
      public:
             
             Family(numtype rootID,numtype familyID):rootid(rootID),familyID(familyID),root(NodeID[rootID]),total_members(0),total_dead(0),total_male(0),total_female(0)
             {}
             
             void add_partner(numtype husbandid, numtype wifeid)
             {
                  Person *husband = NodeID[husbandid];
                  Person *wife = NodeID[wifeid];
                  
                  husband->partner.push_back(wifeid);
                  wife->partner.push_back(husbandid);
             }
             
             void add_children(numtype childid, numtype fatherid, numtype motherid)
             {
                  Person *father = NodeID[fatherid];
                  Person *mother = NodeID[motherid];
                  
                  father->children.push_back(childid);
                  mother->children.push_back(childid);
             }
             
             void add_sibling(numtype p1, numtype p2)
             {
                  Person *bro1 = NodeID[p1];
                  Person *bro2 = NodeID[p2];
                  
                  bro1->sibling.push_back(p2);
                  bro2->sibling.push_back(p1);
             }
             
             pair<numtype,numtype> return_parents(numtype node)
             {
                 Person *p = NodeID[node];
                 return (make_pair(p->father,p->mother));
             }           
};

Family NewFamily(numtype root_node)
{
       Family family(root_node,new_familyid());
       
       FamilyRoots.push_back(root_node);      
       
       return family;
}

class Details{
      
      string dob,name,pattern,input;
      int position;
      bool is_dead,gender;
             
              void getPattern()
              {
                     for(position=0;input[position]!='#';position++)
                     pattern += input[position];
                     
              }
              
              void getName(){
                     int hash_count=0;
                     
                     while(hash_count<3)
                     {
                                        if(input[position]=='#') hash_count++;
                                        else name += input[position];
                                        
                                        position++;
                     }
              }
              
              void getGender(){
                   size_t found = input.find('Female');
                   
                   if (found!=std::string::npos)
                   gender=0;
                   else gender=1;
              }
              
              void isDead(){
                   
                   if(pattern[pattern.size()-1]=='.' && pattern.size()!=1) is_dead= true;
                   else is_dead=false;
              }
              
              void getDOB(){
                   
                   int i=input.size()-1;
                   
                   while(!static_cast<int>(input[i])>=48 && !static_cast<int>(input[i])<=57)
                   {i--;}
                   
                   while(input[i]!='#'){i--;}
                   
                   while(input[i]!='#'){dob+=input[i]; i++;}          
              } 
              
      public:
             
             Details(string inp):input(inp),name(""),pattern(""),is_dead(0),position(0){}
             
             void computeDetails(){
                  getPattern();
                  getName();
                  getGender();
                  isDead();
                  getDOB();
             }
             
             string Pattern(){ return pattern; }
             string Name(){ return name; }
             bool Gender(){ return gender;}
             bool Dead(){ return is_dead;}
             string DOB(){ return dob;}    
};                      

void print_stack(stack<nns > s)
{
     while(!s.empty()){
                       cout<<NodeID[s.top().f.f]->name<<" "<<NodeID[s.top().f.s]->name<<" "<<s.top().s<<endl;
                       s.pop();
     }
     cout<<endl;
}

void print_tree(int node){
     int i;
     Person *current = NodeID[node];
     
     cout<<current->name<<endl;
     
     vector<numtype> partner = current->partner;          
     vector<numtype> children = current->children;
     
     for(i=0;i<partner.size();i++) cout << NodeID[partner[i]]->name<<",";
     
     cout<<endl;
     
     for(i=0;i<children.size();i++) {cout<<"  ";print_tree(children[i]);
        //cout<<NodeID[children[i]]->name<<" ";
     }
     
     return;
}
  
void space(stack<nns > temp_stack){
     
     for(int i=0;i<temp_stack.size();i++) cout<<" ";
     }
        
int main()
{
    ifstream inf("testdata.txt");

    string line;
    
    Family family(-1,-1);
    stack<nns > temp_stack;
    int count=0;
    while(count<50)
    {              count++;
            getline(inf,line);
            
            //cout<<line<<endl;
            
            Details d(line);
            d.computeDetails();
            
            string pattern = d.Pattern();
            
            int relation = Relation(pattern,temp_stack);    
               
               //print_stack(temp_stack);
               
               if(relation==0)
               {
                    //space(temp_stack);cout<<"Root: ";
                    int new_node=new_Person(d.Name(),d.DOB(),d.Gender(),d.Dead(),-1,-1);
                    nns new_top = make_pair(make_pair(new_node,-1),pattern);
                    temp_stack.push(new_top);
                    
                    family = NewFamily(new_node);  
                    
                    cout<<d.Name()<<endl;        
               }
               else if(relation==1)
               {    
                    //space(temp_stack);cout<<"wife :";
                    int new_node=new_Person(d.Name(),d.DOB(),d.Gender(),d.Dead(),-1,-1);
                    nns top = temp_stack.top(); temp_stack.pop();
                    top.f.s = new_node;
                    temp_stack.push(top);
                    
                    family.add_partner(top.f.f,top.f.s); 
                    cout<<d.Name()<<endl;      
               }
               else if(relation==2)
               {     
                    //space(temp_stack);cout<<"child :";
                    nns top = temp_stack.top(); 
                    int new_node=new_Person(d.Name(),d.DOB(),d.Gender(),d.Dead(),top.f.f,top.f.s);
                    
                    family.add_children(new_node,top.f.f,top.f.s);
                    
                    nns new_top = make_pair(make_pair(new_node,-1),pattern);
                    temp_stack.push(new_top);  
                    
                    cout<<d.Name()<<endl;   
               }
               else if(relation==3)
               {
                    //space(temp_stack);cout<<"sibling :";
                    int new_node=new_Person(d.Name(),d.DOB(),d.Gender(),d.Dead(),-1,-1);
                    
                    nns top = temp_stack.top();
                    
                    stack<nns >sibling_stack;                    
                    
                    while(Relation(pattern,temp_stack)==3)
                    {   
                        family.add_sibling(new_node,top.f.f);
                        sibling_stack.push(top);
                        temp_stack.pop();
                        
                        top=temp_stack.top();
                    }
                    
                    family.add_children(new_node,top.f.f,top.f.s);
                    
                    copyBackStack(sibling_stack,temp_stack);
                    
                    nns new_top = make_pair(make_pair(new_node,-1),pattern);
                    temp_stack.push(new_top); 
                    cout<<d.Name()<<endl;     
               }
               else if(relation==4)
               {
                   // space(temp_stack);cout<<"child :";
                    int new_node=new_Person(d.Name(),d.DOB(),d.Gender(),d.Dead(),-1,-1);
                    
                    while(Relation(pattern,temp_stack)!=3)
                    {
                          temp_stack.pop();
                    }
                    
                    nns top = temp_stack.top();
                    
                    family.add_sibling(new_node,top.f.f);
                    
                    pair<numtype,numtype> parents= family.return_parents(top.f.f);
                    
                    family.add_children(new_node,parents.f,parents.s);
                    
                    cout<<NodeID[parents.f]->name<<" ";
                    
                    nns new_top = make_pair(make_pair(new_node,-1),pattern);
                    temp_stack.push(new_top);
                    cout<<d.Name()<<endl; 
               }
               else if(relation==-1)
               {
                    cout<<"Invalid Input"<<endl;     
               }        
    }                                                                 
    
    print_tree(-1);
                 
    getch();
}
    
