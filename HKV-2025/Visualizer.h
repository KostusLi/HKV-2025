#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <stack>
#include <map>

namespace Visualizer
{
    struct Node {
        int id;
        std::string label;
        bool isNonTerminal;
    };

    class TreeBuilder {
    private:
        int nodeCounter = 0;
        std::vector<std::pair<int, int>> edges; // Родитель -> Ребенок
        std::map<int, Node> nodes;
        std::stack<int> nonTerminalStack; // Стек ID узлов, ожидающих раскрытия

        int addNode(std::string label, bool isNonTerminal);
        void addEdge(int parentId, int childId);

    public:
        TreeBuilder();
        // Метод, который парсит одну строку правила (например: "S->pfiPGS")
        void processRule(std::string ruleStr);

        // Метод, который сохраняет результат в .dot файл и генерирует картинку
        void generateGraphImage(std::string dotFilename, std::string pngFilename);
    };
}