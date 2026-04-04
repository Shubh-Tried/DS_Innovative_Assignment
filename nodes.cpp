#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>

#include "./BTree.h"

using namespace std;

vector<schema> showEle(struct index* data, int emp_salary, char c = 's'){
  vector<schema> v;
  for(schema i: data->data){
    if(i.emp_salary == emp_salary && c == 's'){
      v.push_back(i);
      return v;
    }
    else if(i.emp_salary >= emp_salary && c == 'g')
      v.push_back(i);
    
    else if(i.emp_salary <= emp_salary && c == 'l')
      v.push_back(i);

  }
  return v;
}

int main() {

}