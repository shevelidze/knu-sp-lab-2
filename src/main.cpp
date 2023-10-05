#include <iostream>
#include <fstream>
#include <set>
#include <vector>

class AutomataTransition
{
public:
  AutomataTransition(char symbol, int inputState, int outputState)
  {
    this->symbol = symbol;
    this->inputState = inputState;
    this->outputState = outputState;
  }

  char symbol;
  int inputState;
  int outputState;

  static AutomataTransition read(std::istream &input_stream)
  {
    char symbol;
    int inputState;
    int outputState;

    input_stream >> inputState;
    input_stream >> symbol;
    input_stream >> outputState;

    return AutomataTransition(symbol, inputState, outputState);
  }
};

class Automata
{
public:
  Automata(int startState, std::set<int> finalStates, std::vector<AutomataTransition> transitions)
  {
    this->startState = startState;
    this->finalStates = finalStates;
    this->transitions = transitions;
    this->currentState = startState;
  }

  bool processSymbol(char symbol)
  {
    for (const auto &transition : transitions)
    {
      if (transition.symbol == symbol && this->currentState == transition.inputState)
      {
        this->currentState = transition.outputState;
        return true;
      }
    }

    return false;
  }

  bool processWord(std::string word)
  {
    for (const auto &symbol : word)
    {
      if (!processSymbol(symbol))
      {
        return false;
      }
    }

    return true;
  }

  void resetState()
  {
    this->currentState = this->startState;
  }

  bool isStateFinal()
  {
    return this->finalStates.find(this->currentState) != this->finalStates.end();
  }

  int startState;
  std::set<int> finalStates;
  std::vector<AutomataTransition> transitions;
  int currentState;
};

int readNumber(std::istream &input_stream)
{
  int power;
  input_stream >> power;
  return power;
}

std::set<int> readFinalStates(std::istream &input_stream)
{
  const auto statesNumber = readNumber(input_stream);

  std::set<int> finalStates;

  for (int i = 0; i < statesNumber; i++)
  {
    finalStates.insert(readNumber(input_stream));
  }

  return finalStates;
}

std::vector<char> getAlphabet(const int &power)
{
  std::vector<char> alphabet;

  for (int i = 0; i < power; i++)
  {
    alphabet.push_back('a' + i);
  }

  return alphabet;
}

std::vector<int> getStates(const int &power)
{
  std::vector<int> states;

  for (int i = 0; i < power; i++)
  {
    states.push_back(i);
  }

  return states;
}

bool search(const Automata &automata, std::set<int> &pastStates, std::vector<char> alphabet, std::set<int> acceptableStates)
{
  if (pastStates.find(automata.currentState) != pastStates.end())
  {
    return false;
  }

  pastStates.insert(automata.currentState);

  for (const auto &symbol : alphabet)
  {
    Automata newAutomata(automata);

    if (!newAutomata.processSymbol(symbol))
    {
      continue;
    }

    if (acceptableStates.find(newAutomata.currentState) != acceptableStates.end())
    {
      return true;
    }

    if (search(newAutomata, pastStates, alphabet, acceptableStates))
    {
      return true;
    }
  }

  return false;
}

bool algorithm(std::string w1, std::string w2, const Automata &automata, const int &alphabetPower, const int &statesPower)
{
  std::set<int> w2States;

  for (const auto &state : getStates(statesPower))
  {
    Automata tempAutomata(automata);

    if (tempAutomata.processWord(w2) && tempAutomata.isStateFinal())
    {
      w2States.insert(state);
    }
  }

  Automata w1Automata(automata);
  w1Automata.processWord(w1);

  std::set<int> pastStates;

  return search(w1Automata, pastStates, getAlphabet(alphabetPower), w2States);
}

int main(int argc, char **argv)
{
  std::ifstream inputFile;

  inputFile.open(argv[1]);

  if (!inputFile.is_open())
  {
    std::cout << "Error opening file" << std::endl;
    return 1;
  }

  auto alphabetPower = readNumber(inputFile);
  auto statesPower = readNumber(inputFile);

  const auto startState = readNumber(inputFile);

  const auto finalStates = readFinalStates(inputFile);

  std::vector<AutomataTransition> transitions;

  while (inputFile.peek() != EOF)
  {
    transitions.push_back(AutomataTransition::read(inputFile));
  }

  const std::string w1 = "ea", w2 = "d";

  const bool result = algorithm(w1, w2, Automata(startState, finalStates, transitions), alphabetPower, statesPower);

  const std::string resultString = result ? "true" : "false";

  std::cout << "Result: " << resultString << std::endl;

  return 0;
}
