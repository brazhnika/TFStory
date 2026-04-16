#pragma once
#include <cassert>
#include <iostream>
#include <sstream>


template<typename T>
struct SpriteYPos
{
    SpriteYPos* next{nullptr};
    SpriteYPos* prev{nullptr};
    T* m_sprite{nullptr};
    //SpriteYPos(std::shared_ptr<T> sprite)
    //    : m_sprite(sprite)
    //{}
};

template<typename T>
class SortedSpritesList
{
public:

    int size() const { return m_size; }

    template<typename Type, typename PointType>
    class Iterator
    {
    public:
        Iterator(SpriteYPos<T>* current)
            : m_current(current)
        {}

        Iterator<Type, PointType>& operator++()
        {
            if(m_current)
                m_current = m_current->next;
            return *this;
        }

        Iterator<Type, PointType>& operator--()
        {
            if(m_current)
                m_current = m_current->prev;
            return *this;
        }

        PointType operator*()
        {
            if(!m_current) return nullptr;
            return m_current->m_sprite;
        }

        PointType operator->()
        {
            if(!m_current) return nullptr;
            return m_current->m_sprite;
        }

        bool operator==(const Iterator<Type, PointType>& other)
        {
            return m_current == other.m_current;
        }

        bool operator!=(const Iterator<Type, PointType>& other)
        {
            return !(*this == other);
        }

    private:
        SpriteYPos<T>* m_current;
    };

    using ConstIteratorT = Iterator<const T, const T*>;
    using IteratorT = Iterator<T, T*>;

    ConstIteratorT cbegin() const { return ConstIteratorT(m_head);}
    ConstIteratorT cend() const { return ConstIteratorT(nullptr);}
    ConstIteratorT crbegin() const { return ConstIteratorT(m_tail);}
    ConstIteratorT crend() const { return ConstIteratorT(nullptr);}

    IteratorT begin() const { return IteratorT(m_head);}
    IteratorT end() const { return IteratorT(nullptr);}
    IteratorT rbegin() const { return IteratorT(m_tail);}
    IteratorT rend() const { return IteratorT(nullptr);}    

    void insert(SpriteYPos<T>* spriteYElem)
    {
        //std::cout << "Inserting sprite " << spriteYElem->m_sprite->GetGlobalY();
        assert(spriteYElem);        
        spriteYElem->next = nullptr;
        spriteYElem->prev = nullptr;
        spriteYElem->m_sprite->SetChangeHandler([this](T& sprite, int oldValue)
           {
               move(sprite, oldValue);
           });

        if(!m_head)
        {
            assert(m_size == 0);
            m_head = spriteYElem;
            assert(!m_tail);
            m_tail = spriteYElem;
            ++m_size;
            assert(m_size > 0);
            //std::cout << "Inserting as head \n";
            return;
        }
        auto firstBigger = getFirstBiggerThan(m_head, spriteYElem->m_sprite->GetGlobalY());
        if(!firstBigger)
        {
            spriteYElem->prev = m_tail;
            m_tail->next = spriteYElem;
            m_tail = spriteYElem;      
            ++m_size; 
        }
        else
           pasteBefore(spriteYElem, firstBigger);
    }

    void move(T& sprite, int oldValue)
    {
        ss = std::stringstream();
        printToSS();
        ss << " Change " << sprite.GetName() << " from " << oldValue << " to " << sprite.GetGlobalY() << "\n";
        //assert(sprite);
        auto newValue = sprite.GetGlobalY();
        if(newValue == oldValue) return;

        SpriteYPos<T>* elemToMove = sprite.GetYPosListElem();
        if(newValue > oldValue)
        {
            if(elemToMove == m_tail) return;
            
            //std::cout << " newValue " << newValue << " > " << " oldValue " << oldValue << "\n";
            auto firstBigger = getFirstBiggerThan(elemToMove->next, newValue);
            cut(elemToMove);
            if(firstBigger)
                pasteBefore(elemToMove, firstBigger);
            else
                pasteAfterTail(elemToMove);
        }
        else
        {
            //std::cout << " newValue " << newValue << " < " << " oldValue " << oldValue << "\n";
            if(elemToMove == m_head) return;

            auto firstLess = getFirstLessThan(elemToMove->prev, newValue);
            cut(elemToMove);        
            if(firstLess)
            {
                ss << " found firstLess = " << firstLess->m_sprite->GetName() << "("<<firstLess->m_sprite->GetGlobalY() << ")\n";
                pasteAfter(elemToMove, firstLess);
            }
            else
            {
                pasteBeforeHead(elemToMove);
            }
        }
    }

    void print(bool forward)
    {
        if (forward)
        {
           auto bI = m_head;
           while(bI != nullptr)
           {
               std::cout << ", " << bI->m_sprite->GetName() <<"=" << bI->m_sprite->GetGlobalY();
               bI = bI->next;
           }
           std::cout << "\n\n";
        }
        else
        {
           auto bI = m_tail;
           while(bI != nullptr)
           {
               std::cout << ", " << bI->m_sprite->GetName() <<"=" << bI->m_sprite->GetGlobalY();
               bI = bI->prev;
           }
           std::cout << "\n\n";
        }
    }

    void printToSS()
    {
        auto bI = m_head;
        while(bI != nullptr)
        {
            ss << ", " << bI->m_sprite->GetName() <<"=" << bI->m_sprite->GetGlobalY();
            bI = bI->next;
        }
        ss << "\n\n backward: ";

        bI = m_tail;
        while(bI != nullptr)
        {
            ss << ", " << bI->m_sprite->GetName() <<"=" << bI->m_sprite->GetGlobalY();
            bI = bI->prev;
        }
    }
   
private:
    SpriteYPos<T>* getFirstBiggerThan(SpriteYPos<T>* from, int yValue)
    {
        ss << "getFirstBiggerThan y=" << yValue << "\n";
        //std::cout << "getFirstBiggerThan y=" << yValue << "\n";
        assert(from);
        auto tmp = from;
        while((tmp != nullptr) && (tmp->m_sprite->GetGlobalY() < yValue))
        {
            //std::cout << " next->m_sprite->GetGlobalY() " << tmp->m_sprite->GetGlobalY() << "\n";
            tmp = tmp->next;
        }
        return tmp;
    }

    SpriteYPos<T>* getFirstLessThan(SpriteYPos<T>* from, int yValue)
    {
        //std::cout << "getFirstLessThan y=" << yValue << "\n";
        ss <<  "getFirstLessThan y=" << yValue << "\n";
        assert(from);
        auto tmp = from;
        while((tmp != nullptr) && (tmp->m_sprite->GetGlobalY() > yValue))
        {
            ss <<  "prev  " <<  tmp->m_sprite->GetName() << " " << tmp->m_sprite->GetGlobalY() << "\n";
            tmp = tmp->prev;
        }
        return tmp;
    }
    
    void cut(SpriteYPos<T>* pos)
    {
        assert(pos);
        if(pos == m_head)
        {
            assert(pos->next && (m_head != m_tail));
            m_head = pos->next;
            m_head->prev = nullptr;
        }
        else if (pos == m_tail)
        {
            assert(pos->prev && (m_head != m_tail));
            m_tail = pos->prev;
            m_tail->next = nullptr;
        }
        else
        {
            assert(pos->next && pos->prev);
            pos->prev->next = pos->next;
            pos->next->prev = pos->prev;
        }
        pos->next = nullptr;
        pos->prev = nullptr;
        --m_size;
        assert(m_size >= 0);
    }

    void pasteBefore(SpriteYPos<T>* itemToInsert, SpriteYPos<T>* nextItem)
    {
        //std::cout << itemToInsert->m_sprite->GetGlobalY() << " pasteBefore " << nextItem->m_sprite->GetGlobalY() << "\n";
        assert(itemToInsert);
        if(nextItem == m_head)
        {
            pasteBeforeHead(itemToInsert);
        }
        else
        {
            nextItem->prev->next = itemToInsert;
            itemToInsert->prev = nextItem->prev;
            itemToInsert->next = nextItem;
            nextItem->prev = itemToInsert;
            ++m_size;
        }
        assert(m_size > 0);
    }

    void pasteAfter(SpriteYPos<T>* itemToInsert, SpriteYPos<T>* nextItem)
    {
        assert(itemToInsert);
        if(nextItem == m_tail)
        {
            pasteAfterTail(itemToInsert);
        }
        else
        {
            nextItem->next->prev = itemToInsert;
            itemToInsert->next = nextItem->next;
            itemToInsert->prev = nextItem;
            nextItem->next = itemToInsert;
            ++m_size;
        }
        assert(m_size > 0);        
    }

    void pasteBeforeHead(SpriteYPos<T>* itemToInsert)
    {
        assert(itemToInsert && m_head);
        if(m_head->m_sprite->GetGlobalY() < itemToInsert->m_sprite->GetGlobalY())
        {
            std::cout << " m_head " << m_head->m_sprite->GetName() << "(" << m_head->m_sprite->GetGlobalY() << ")"
                << " itemToInsert " <<  itemToInsert->m_sprite->GetName() << "("  << itemToInsert->m_sprite->GetGlobalY() << ")"
                << "\n";
            std::cout << ss.str() << "\n";
            //print();
            assert(false);
        }
        //assert(m_head->m_sprite->GetGlobalY() >=  itemToInsert->m_sprite->GetGlobalY());
        itemToInsert->next = m_head;
        m_head->prev = itemToInsert;
        m_head = itemToInsert;
        ++m_size;
    }

    void pasteAfterTail(SpriteYPos<T>* itemToInsert)
    {
        assert(itemToInsert && m_tail);
        assert(m_tail->m_sprite->GetGlobalY() <=  itemToInsert->m_sprite->GetGlobalY());
        itemToInsert->prev = m_tail;
        m_tail->next = itemToInsert;
        m_tail = itemToInsert;
        ++m_size;
    }

    SpriteYPos<T>* m_head{nullptr};
    SpriteYPos<T>* m_tail{nullptr};
    int m_size{0};
    std::stringstream ss;
};