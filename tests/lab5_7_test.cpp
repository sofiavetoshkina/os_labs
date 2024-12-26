#include <gtest/gtest.h>
#include "BalancedTree.h"
#include "CalculationNode.h"

// Тестирование структуры BalancedTree
TEST(BalancedTreeTests, AddAndExist) {
    BalancedTree tree;

    EXPECT_FALSE(tree.Exist(1));
    tree.AddInTree(1, -1);  // Добавляем узел
    EXPECT_TRUE(tree.Exist(1));  // Узел должен существовать

    tree.AddInTree(2, 1);  // Добавляем дочерний узел
    EXPECT_TRUE(tree.Exist(2));
}

TEST(BalancedTreeTests, RemoveAndAvailability) {
    BalancedTree tree;
    tree.AddInTree(1, -1);
    tree.AddInTree(2, 1);

    EXPECT_TRUE(tree.Exist(2));
    tree.RemoveFromRoot(2);
    EXPECT_FALSE(tree.Exist(2));
}

// Тестирование CalculationNode
TEST(CalculationNodeTests, CreateNode) {
    CalculationNode parentNode(1, 4040, -1);
    std::string result = parentNode.create(2);

    EXPECT_EQ(result.substr(0, 3), "Ok:");  // Успешное создание
    EXPECT_EQ(parentNode.getLeftId(), 2);  // Узел добавлен слева
}

TEST(CalculationNodeTests, ExecCommand) {
    CalculationNode node(1, 4040, -1);
    std::string result = node.exec("3 1 2 3");

    EXPECT_EQ(result, "Ok: 1: 6");  // Проверяем корректность результата
}

TEST(CalculationNodeTests, PingNode) {
    CalculationNode node(1, 4040, -1);
    EXPECT_EQ(node.ping(1), "Ok: 1");  // Узел пингует сам себя
    EXPECT_EQ(node.ping(2), "Ok: 0");  // Неизвестный узел
}

TEST(CalculationNodeTests, KillNode) {
    CalculationNode parentNode(1, 4040, -1);
    parentNode.create(2);  // Создаем дочерний узел

    std::string result = parentNode.kill();
    EXPECT_EQ(result, "-1");  // Проверка корректности удаления узлов
}

// Дополнительные тесты для нестандартных сценариев
TEST(BalancedTreeTests, AddToNonExistingParent) {
    BalancedTree tree;

    // Попытка добавления узла к несуществующему родителю
    EXPECT_NO_THROW(tree.AddInTree(1, 5));  // Ожидается, что программа не упадет
    EXPECT_FALSE(tree.Exist(1));  // Узел не добавлен
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}