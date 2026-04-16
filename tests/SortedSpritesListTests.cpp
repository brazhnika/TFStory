#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "../Utils/bubbleSort.h"
#include "TestData.h"
#include "../src/SortedSpritesList.h"

namespace
{
class SpriteMock
{
public:
    int m_y;
    SpriteYPos<SpriteMock> m_spriteYPos;
    std::function<void(SpriteMock&, int)> m_changeHandler;

    SpriteMock(int y) : m_y(y), m_spriteYPos() {}

    int GetGlobalY() const { return m_y; }
    SpriteYPos<SpriteMock>* GetYPosListElem()
    {
        m_spriteYPos.m_sprite = this;
        return &m_spriteYPos;
    }
    void SetChangeHandler(std::function<void(SpriteMock&, int)> handler) { m_changeHandler = handler;} 
    int GetName(){return m_y;}
};

using SListT = SortedSpritesList<SpriteMock>;

void FillList(SListT& list, std::vector<SpriteMock>& sprites, const std::vector<int>& vec)
{
    for(auto item : vec)
        sprites.emplace_back(item);
    
    ASSERT_EQ(vec.size(), sprites.size());

    list = SListT();        
    for(auto& sprite : sprites)
        list.insert(sprite.GetYPosListElem());
}

void AreEqual(const SListT& list, const std::vector<int>& vec)
{
    ASSERT_EQ(vec.size(), list.size());
    int i = 0;
    for(auto it = list.cbegin(); it != list.cend(); ++it, ++i)
       ASSERT_EQ(vec[i], it->GetGlobalY());
}

void AreEqualBackward(const SListT& list, const std::vector<int>& vec)
{
    ASSERT_EQ(vec.size(), list.size());
    int i = vec.size() - 1;
    for(auto it = list.crbegin(); it != list.crend(); --it, --i)
       ASSERT_EQ(vec[i], it->GetGlobalY());
}

void testInsert(std::vector<int> testData, int insertValue)
{
    SListT list;
    std::vector<SpriteMock> sprites;
    FillList(list, sprites, testData);

    auto newSprite = SpriteMock(insertValue);
    list.insert(newSprite.GetYPosListElem());

    testData.push_back(insertValue);
    BubbleSort(testData);

    AreEqual(list, testData);
    AreEqualBackward(list, testData);
}


void testMoveToTheBorder(std::vector<int> testData,
    int moveId, std::vector<int> result, bool moveToTheBeginning)
{
    SListT list;
    std::vector<SpriteMock> sprites;
    FillList(list, sprites, testData);
    auto oldValue = sprites[moveId].m_y;
    auto newValue = moveToTheBeginning
        ? std::min_element(testData.begin(), testData.end())
        : std::max_element(testData.begin(), testData.end());
    sprites[moveId].m_y = moveToTheBeginning ? -*newValue : *newValue * 2;
    sprites[moveId].m_changeHandler(sprites[moveId], oldValue);
    AreEqual(list, result);
    AreEqualBackward(list, result);    
}

// void print(const SListT& list)
// {
//     for(auto it = list.cbegin(); it != list.cend(); ++it)
//     {
//         std::cout << " list " << it->GetGlobalY() << "\n";
//     }
// }

}// anonymus namespace

TEST(SortedSpritesList, EmptyListTest)
{
    for(auto data : GetTestIntData())
    {
       SListT list;
       std::vector<SpriteMock> sprites;
       FillList(list, sprites, data.data);
       AreEqual(list, data.sortedData);
    }
}


TEST(SortedSpritesList, InsertAtTheBeginning)
{
    testInsert({10, 4, 15, 34, 17, 2, 3, 6, 3, 8, 3, 10}, -1 /*insertValue*/);
}

TEST(SortedSpritesList, InsertAtTheEnd)
{
    testInsert({10, 4, 15, 34, 17, 2, 3, 6, 3, 8, 3, 10}, 100 /*insertValue*/);    
}

TEST(SortedSpritesList, InsertInTheMiddle)
{
    testInsert({10, 4, 15, 34, 17, 2, 3, 6, 3, 8, 3, 10}, 14 /*insertValue*/);    
}

TEST(SortedSpritesList, MoveMiddleAtTheEnd)
{
    testMoveToTheBorder({10, 4, 15, 34, 17, 2, 3, 6, 3, 8, 3, 10}, 1 /*moveId*/,
         {2, 3, 3, 3, 6, 8, 10, 10, 15, 17, 34, 68}, false /*moveToTheBeginning*/);
}

TEST(SortedSpritesList, MoveEndAtTheEnd)
{
    testMoveToTheBorder({10, 4, 15, 34, 17, 2, 3, 6, 3, 8, 3, 10}, 3 /*moveId*/,
        {2, 3, 3, 3, 4, 6, 8, 10, 10, 15, 17, 68}, false /*moveToTheBeginning*/);
}

TEST(SortedSpritesList, MoveBeginAtTheBegin)
{
    testMoveToTheBorder({10, 4, 15, 34, 17, 2, 3, 6, 3, 8, 3, 10}, 5 /*moveId*/,
        {-2, 3, 3, 3, 4, 6, 8, 10, 10, 15, 17, 34}, true /*moveToTheBeginning*/);    
}

TEST(SortedSpritesList, MoveMiddleAtTheBegin)
{
    testMoveToTheBorder({10, 4, 15, 34, 17, 2, 3, 6, 3, 8, 3, 10}, 0 /*moveId*/,
        {-2, 2, 3, 3, 3, 4, 6, 8, 10, 15, 17, 34}, true /*moveToTheBeginning*/);       
}

// TEST(SortedSpritesList, CheckListIsLinked)
// {
//     std::vector<int> testData{10, 4, 15, 34, 17, 2, 3, 6, 3, 8, 3, 10, 66 , 23, 1500, 1555, -2};
//     SListT list;
//     std::vector<SpriteMock> sprites;
//     FillList(list, sprites, testData);
//     //AreEqual(list, data.sortedData);
// }