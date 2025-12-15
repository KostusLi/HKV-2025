#include "Visualizer.h"
#include <iostream>
#include <sstream>
#include <algorithm>

namespace Visualizer
{
    TreeBuilder::TreeBuilder() {}

    int TreeBuilder::addNode(std::string label, bool isNonTerminal) {
        int id = nodeCounter++;
        nodes[id] = { id, label, isNonTerminal };
        return id;
    }

    void TreeBuilder::addEdge(int parentId, int childId) {
        edges.push_back({ parentId, childId });
    }

    void TreeBuilder::processRule(std::string ruleStr) {
        // Ожидаемый формат строки, который мы парсим из лога: "S->pfiPGS"
        // (Номер шага и двоеточие мы отсечем до вызова)

        size_t arrowPos = ruleStr.find("->");
        if (arrowPos == std::string::npos) return;

        std::string leftSide = ruleStr.substr(0, arrowPos);
        std::string rightSide = ruleStr.substr(arrowPos + 2);

        // Очистка от пробелов
        leftSide.erase(remove(leftSide.begin(), leftSide.end(), ' '), leftSide.end());
        rightSide.erase(remove(rightSide.begin(), rightSide.end(), ' '), rightSide.end());

        int parentId;

        if (nodes.empty()) {
            // Если это самое первое правило, создаем корень
            parentId = addNode(leftSide, true);
        }
        else {
            // Если не первое, берем ожидающий нетерминал из стека
            if (nonTerminalStack.empty()) {
                // Ошибка логики или конец разбора, но попробуем продолжить
                return;
            }
            parentId = nonTerminalStack.top();
            nonTerminalStack.pop();

            // Проверка: метка в стеке должна совпадать с левой частью правила
            if (nodes[parentId].label != leftSide) {
                // Это может случиться при backtracking-е в логе, 
                // но для успешного разбора должно совпадать.
                // В твоем случае просто перезапишем метку для красоты, если вдруг расхождение
                // (хотя это сигнал о странности в логе)
            }
        }

        // Обрабатываем правую часть (дети)
        // Идем справа налево, чтобы правильно засунуть в стек (LIFO)
        // Но добавляем ребра в правильном порядке
        std::vector<int> childIds;

        // В твоей грамматике односимвольные токены, кроме имен нетерминалов и терминалов?
        // В GRB у тебя все Chains состоят из одиночных char-ов (TS('t'), NS('P')).
        // Значит, мы можем читать строку посимвольно.

        for (char c : rightSide) {
            std::string label(1, c);

            // Определяем, нетерминал это или терминал.
            // В твоей грамматике нетерминалы - заглавные буквы (S, P, G, E, T, K, V, F, N, R, Y, Z, L, W, A, J, X)
            // Терминалы - строчные или спецсимволы.
            bool isNT = (c >= 'A' && c <= 'Z');

            int childId = addNode(label, isNT);
            addEdge(parentId, childId);
            childIds.push_back(childId);
        }

        // Теперь кладем Нетерминалы в стек В ОБРАТНОМ ПОРЯДКЕ (справа налево),
        // чтобы левый нетерминал оказался на вершине стека для следующего шага.
        for (auto it = childIds.rbegin(); it != childIds.rend(); ++it) {
            if (nodes[*it].isNonTerminal) {
                nonTerminalStack.push(*it);
            }
        }
    }

    void TreeBuilder::generateGraphImage(std::string dotFilename, std::string pngFilename) {
        std::ofstream out(dotFilename);
        if (!out.is_open()) return;

        out << "digraph ParseTree {\n";
        out << "    node [shape=box, fontname=\"Arial\"];\n";
        out << "    edge [color=\"black\"];\n";

        // Описываем узлы
        for (auto const& [id, node] : nodes) {
            std::string shape = node.isNonTerminal ? "ellipse" : "circle"; // Нетерминалы - овалы, терминалы - квадраты
            std::string color = node.isNonTerminal ? "yellow" : "green";
            std::string style = "filled";

            // Экранирование кавычек для меток типа "
            std::string label = node.label;
            if (label == "\"") label = "\\\"";

            out << "    node" << id << " [label=\"" << label << "\", shape=" << shape
                << ", style=" << style << ", fillcolor=" << color << "];\n";
        }

        // Описываем связи
        for (const auto& edge : edges) {
            out << "    node" << edge.first << " -> node" << edge.second << ";\n";
        }

        out << "}\n";
        out.close();

        // Вызов команды Graphviz
        std::string command = "dot -Tpng " + dotFilename + " -o " + pngFilename;
        std::cout << "Generating tree image: " << command << std::endl;

        // Запуск
        int result = system(command.c_str());

        if (result == 0) {
            std::cout << "Tree visualization saved to " << pngFilename << std::endl;
            // Можно даже открыть файл сразу:
            system(("start " + pngFilename).c_str());
        }
        else {
            std::cout << "Error executing Graphviz. Make sure 'dot' is in your PATH." << std::endl;
        }
    }
}