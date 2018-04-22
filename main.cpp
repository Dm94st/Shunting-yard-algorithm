#include <iostream>
#include <stack>
#include <string>
#include <map>

/*
Hight: * /
Medium: + −
Low: ( )
*/

enum Priority
{
  HIGHT = 2,
  MED = 1,
  LOW = 0
};

enum Brackets
{
  OPEN_BRACKET = '(',
  CLOSE_BRACKET = ')'
};

class OpPriority
{
public:
  explicit OpPriority()
  {
    Init();
  }

  int FindPriorByOp(char op) const
  {
    int prior;
    auto find = opPrior.find(op);
    if (find != opPrior.end())
    {
      prior = find->second;
      return prior;
    }
    else
      throw std::invalid_argument("Unknown operator!");
  }

  bool isOperator(char sym) const
  {
    bool isFound = false;
    auto find = opPrior.find(sym);
    if (find != opPrior.end())
    {
      isFound = true;
      return isFound;
    }
    return isFound;
  }

private:
  void Init()
  {
    //Hight priority
    opPrior['*'] = Priority::HIGHT;
    opPrior['/'] = Priority::HIGHT;
    //Med priority
    opPrior['+'] = Priority::MED;
    opPrior['-'] = Priority::MED;
    //Low priority
    opPrior['('] = Priority::LOW;
    opPrior[')'] = Priority::LOW;
  }
  std::map<char,int> opPrior;
}
;

class Parser
{
public:
  Parser(const std::string &srcFormula):m_srcFormula(srcFormula), m_op()
  {}

  int Parse()
  {
    std::string postfixForm = FromInfixToPostfix();
    int result = EvaluatePostfix(postfixForm);
    return result;
  }

private:
  std::string m_srcFormula;
  OpPriority m_op;

  std::string FromInfixToPostfix() const
  {
    std::string strOut;
    std::stack<char> stackOp;

    for(auto curSym = m_srcFormula.begin(); curSym != m_srcFormula.end(); ++curSym)
    {
      // If current symbol is digit
      if(std::isdigit(*curSym))
      {
        strOut += *(curSym);
      }
      // If current symbol is open bracket
      else if(*curSym == Brackets::OPEN_BRACKET)
      {
        stackOp.push(*curSym);
      }
      // If current symbol is close bracket
      else if(*curSym == Brackets::CLOSE_BRACKET)
      {
        while(stackOp.top() != Brackets::OPEN_BRACKET)
        {
          if(!stackOp.empty())
          {
            strOut += stackOp.top();
            stackOp.pop();
          }
          else
            throw std::invalid_argument("Incorrect expression!");
        }
        stackOp.pop(); // Remove open bracket
      }
      // If current symbol is operator and stack is empty
      else if(m_op.isOperator(*curSym) && stackOp.empty())
      {
        stackOp.push(*curSym);
      }
      // If current symbol is operator and stack is not empty
      else if(m_op.isOperator(*curSym) && !stackOp.empty())
      {
        int topOpPrior = m_op.FindPriorByOp(stackOp.top()); //Priority of operator of TOP stack
        int curOpPrior = m_op.FindPriorByOp(*curSym); //Priority of operator of current symbol

        while((topOpPrior >= curOpPrior) && !stackOp.empty())
        {
          strOut += stackOp.top();
          stackOp.pop();

          if(!stackOp.empty())
            topOpPrior = m_op.FindPriorByOp(stackOp.top());
          else
            continue;

          curOpPrior = m_op.FindPriorByOp(*curSym);
        }
        stackOp.push(*curSym);
        strOut += std::string(" ");
      }
    }
    //Add to string remaining in stack operators
    while(!stackOp.empty())
    {
      strOut += stackOp.top();
      stackOp.pop();
    }
    return strOut;
  }

  // Calculate postfix expression
  int EvaluatePostfix(const std::string &postfixForm) const
  {
    std::stack<int> st;
    std::string tmpBuf;

    for(auto curSym = postfixForm.begin(); curSym != postfixForm.end(); ++curSym)
    {
      if(std::isdigit(*curSym))
      {
        tmpBuf += *curSym;
      }
      else
      {
        if(!tmpBuf.empty())
        {
          st.push(std::stoi(tmpBuf));
        }
        tmpBuf.clear();
      }

      if(m_op.isOperator(*curSym) && !st.empty())
      {
        int res = 0;

        int rightOp = st.top(); // Last value from stack
        st.pop();
        int leftOp = st.top();
        st.pop();

        switch(*curSym)
        {
        case '*':
          res = leftOp * rightOp;
          break;
        case '/':
          res = leftOp / rightOp;
          break;
        case '+':
          res = leftOp + rightOp;
          break;
        case '-':
          res = leftOp - rightOp;
          break;
        }
        st.push(res);
      }
    }
    return st.top();
  }
}
;

int main(void)
{
  // Example
  std::string srcFormula = "(12+13)*3+(((2*3)+5)+8+(3+4))+((1+4)*5)";
  std::string srcFormula2 = "(2+3) * 4 - 5";

  try
  {
    Parser p(srcFormula);
    int answer = p.Parse();
    std::cout<<answer<<std::endl; //126
  }
  catch(std::exception const &ex)
  {
    std::cerr << ex.what()<<std::endl;
  }
}
