#include <iostream>
#include <vector>
#include <map>
#include <string>

class Node {
public:
	virtual ~Node() {};
	virtual std::string codeGen() {}
};

class Name : public Node {
public:
	std::string name;
	bool open;
	bool single;
	std::vector<Node> children;
	std::map<String,String> params;
	Name(_open=true, _single=false) : open(_open), single(_single) {}
};

class Value : public Node {
public:
	std::string value;
};

