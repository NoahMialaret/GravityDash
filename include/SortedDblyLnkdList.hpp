#ifndef SORTEDDBLYLINKLIST_HPP
#define SORTEDDBLYLINKLIST_HPP

template <typename T>
class SortedDblyLnkdList
{
private:
  class Node
  {
  public:
    Node(T* data, Node* next, Node* previous)
      :
      data(data),
      next(next),
      previous(previous)
    {}

    T* GetData()
    {
      return data;
    }

    Node* GetNextNode() const
    {
      return next;
    }
    Node* GetPreviousNode() const
    {
      return previous;
    }

    void Delete()
    {
      if (next != nullptr)
        next->previous = previous;
      if (previous != nullptr)
        previous->next = next;
    }

  public:
    T* data;

    Node* next = nullptr;
    Node* previous = nullptr;

  };
  
public:
  SortedDblyLnkdList() = default;
  ~SortedDblyLnkdList();

  void InsertData(T* data, Node* searchFrom = nullptr);
  Node* Delete(Node* deleteNode)
{
  if (deleteNode == startOfList)
  {
    startOfList = deleteNode->next;
  }
  else if (deleteNode == endOfList)
  {
    endOfList = deleteNode->previous;
  }
  else if (startOfList == endOfList)
  {
    startOfList = nullptr;
    endOfList = nullptr;
  }
  Node* temp = deleteNode->next;
  deleteNode->Delete();
  delete deleteNode;
  return temp;
}
  void DeleteAll();
  T* At(Node* node);

  Node* Start() const
  {
    return startOfList;
  }
  Node* End() const
  {
    return endOfList;
  }

private:
  Node* startOfList = nullptr;
  Node* endOfList = nullptr;
};

template <typename T>
inline SortedDblyLnkdList<T>::~SortedDblyLnkdList()
{
  DeleteAll();
}

template <typename T>
inline void SortedDblyLnkdList<T>::InsertData(T* data, Node* searchFrom)
{
  if (searchFrom == nullptr)
  {
    searchFrom = startOfList;
  }

  if (searchFrom == nullptr)
  {
    startOfList = new Node(data, nullptr, nullptr);
    endOfList = startOfList;
    return;
  }

  if (searchFrom == startOfList)
  {
    while (searchFrom != nullptr)
    {
      if (*data <= *(searchFrom->data))
      {
        Node* temp = new Node(data, searchFrom, searchFrom->previous);
        if (searchFrom->previous != nullptr)
          searchFrom->previous->next = temp;
        else
          startOfList = temp;
        searchFrom->previous = temp;
        return;
      }
      
      searchFrom = searchFrom->next;
    }

    Node* temp = new Node(data, nullptr, endOfList);
    endOfList->next = temp;
    endOfList = temp;
  }
  else if (searchFrom == endOfList)
  {
    while (searchFrom != nullptr)
    {
      if (*data >= *(searchFrom->data))
      {
        Node* temp = new Node(data, searchFrom->next, searchFrom);
        if (searchFrom->next != nullptr)
          searchFrom->next->previous = temp;
        else
          endOfList = temp;
        searchFrom->next = temp;
        return;
      }
      
      searchFrom = searchFrom->previous;
    }

    Node* temp = new Node(data, startOfList, nullptr);
    startOfList->previous = temp;
    startOfList = temp;
  }
}

template <typename T>
inline void SortedDblyLnkdList<T>::DeleteAll()
{
  while (startOfList != nullptr)
  {
    Node* nextNode = startOfList->next;
    delete startOfList;
    startOfList = nextNode;
  }
}

template <typename T>
inline T* SortedDblyLnkdList<T>::At(Node* node)
{
  return node->GetData();
}

#endif